/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/09 03:52:12 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

// returns true if the elemnt in clipped[] is empty
bool	is_empty(t_triangle *clipped)
{
	if (clipped->normal.x == 0.0f && clipped->normal.y == 0.0f && clipped->normal.z == 0.0f)
		return (true);
	return (false);
}

int8_t	get_set_free_index(t_index_usage *usage)
{
	int8_t		i;

	i = 0;
	while (i < 64)
	{
		if (((*usage) & (1ULL << i)) == 0)
		{
			*usage |= (1ULL << i);
			return (i);
		}
		i++;
	}
	// fprintf(stderr, "error get_set_free_index\n");
	// exit(1);
	return (-1);
}

int8_t	get_unset_last_used_index(t_index_usage *usage)
{
	int8_t		i;

	i = 63;
	while (i)
	{
		if ((*usage) & (1ULL << i))
		{
			*usage &= ~(1ULL << i);
			return (i);
		}
		i--;
	}
	// fprintf(stderr, "error get_set_free_index\n");
	// exit(1);
	return (-1);
}

int8_t	get_unset_used_index(t_index_usage *usage)
{
	int8_t		i;

	i = 0;
	while (i < 64)
	{
		if (((*usage) & (1ULL << i)) == 1)
		{
			*usage &= ~(1ULL << i);
			return (i);
		}
		i++;
	}
	// fprintf(stderr, "error get_set_free_index\n");
	// exit(1);
	return (-1);
}

void	set_index_usage(t_index_usage *usage, int8_t index, int8_t is_used)
{
	if (is_used)
		*usage |= (1ULL << index);
	else
		*usage &= ~(1ULL << index);
}

/*
	if base_flags[0] -> l1 goes through (WIDTH/HEIGHT)
	if !base_flags[0] -> l1 goes through (0, 0)
	if base_flags[1] -> l1 is along the y axis
	if !base_flags[1] -> l1 is along the x axis
*/
void	fast_line_intersect(const int8_t base_flags[2], t_vec3 p1, t_vec3 *p2)
{
	double	progress;
	double	dist_x;
	double	dist_y;

	dist_x = p2->x - p1.x;
	dist_y = p2->y - p1.y;
	if ((zero_f(dist_x) && !base_flags[1]) || (zero_f(dist_y) && base_flags[1]))
	{
		print_vec3(p1, "p1: ");
		print_vec3(*p2, "p2:");
		exit(0);
	}
	if (base_flags[0])
	{
		if (base_flags[1])
			progress = ((((double)HEIGHT) - 1.0 - p1.y) / (dist_y));
		else
			progress = ((((double)WIDTH) - 1.0 - p1.x) / (dist_x));
	}
	else
	{
		if (base_flags[1])
			progress = (-p1.y / (dist_y));
		else
			progress = (-p1.x / (dist_x));
	}
	p2->x = p1.x + (dist_x) * progress;
	p2->y = p1.y + (dist_y) * progress;
	if (p2->y < 0.0f && p2->y > -0.0001f)
		p2->y = 0.0f;
	if (p2->x < 0.0f && p2->x > -0.0001f)
		p2->x = 0.0f;
	p2->u = (p2->u - p1.u) * progress;
	p2->v = (p2->v - p1.v) * progress;
}

void	clipping_xy(t_triangle clipped[30], t_index_usage *used_indexes, const int8_t flags[2], int8_t cur_index)
{
	int8_t				i;
	int8_t				inside_points;
	int8_t				inside_index[3];

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (clipped[cur_index].p[i].x <= 0.0f && clipped[cur_index].p[i].x >= -0.0001f)
			clipped[cur_index].p[i].x = 0.0f;
		if (clipped[cur_index].p[i].y <= 0.0f && clipped[cur_index].p[i].y >= -0.0001f)
			clipped[cur_index].p[i].y = 0.0f;
		if (!flags[0] && !flags[1] && clipped[cur_index].p[i].x > 0.0f)
			inside_index[inside_points++] = i;
		else if (!flags[0] && flags[1] && clipped[cur_index].p[i].y > 0.0f)
			inside_index[inside_points++] = i;
		else if (flags[0] && !flags[1] && clipped[cur_index].p[i].x < (float)WIDTH - 1.0f)
			inside_index[inside_points++] = i;
		else if (flags[0] && flags[1] &&clipped[cur_index].p[i].y < HEIGHT - 1.0f)
		{
			// if (clipped[cur_index].p[i].y >= 699.00f && clipped[cur_index].p[i].y <= 701.00f)
			// 	printf("here\n");
			inside_index[inside_points++] = i;
		}
		i++;
	}
	if (inside_points == 0)
	{
		set_index_usage(used_indexes, cur_index, 0);
		return ;
	}
	if (inside_points == 3)
	{
		if (flags[0] && flags[1])
		{
			for (int i = 0; i < 3; i++)
			{
				if (!flags[0] && !flags[1] && clipped[cur_index].p[i].x < 0.0f)
				{
					printf("case 1\n");
					print_vec3(clipped[cur_index].p[0], "clipped y HEIGTH");
					print_vec3(clipped[cur_index].p[1], "clipped y HEIGTH");
					print_vec3(clipped[cur_index].p[2], "clipped y HEIGTH");
					break ;
				}
			}
		}
		return ;
	}
	if (inside_points == 1)
	{
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		fast_line_intersect(flags, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index1);
		fast_line_intersect(flags, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index2);
		for (int i = 0; i < 3; i++)
		{
			if (!flags[0] && !flags[1] && clipped[0].p[i].x < 0.0f)
			{
				printf("case 2\n");
				print_vec3(clipped[cur_index].p[0], "clipped y HEIGTH");
				print_vec3(clipped[cur_index].p[1], "clipped y HEIGTH");
				print_vec3(clipped[cur_index].p[2], "clipped y HEIGTH");
				break ;
			}
		}
		return ;
	}
	int8_t second_index;
	second_index = get_set_free_index(used_indexes);
	clipped[second_index] = clipped[cur_index];
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	fast_line_intersect(flags, clipped[cur_index].p[inside_index[0]], (clipped[cur_index].p) + outside_index);
	t_vec3 input = clipped[second_index].p[inside_index[1]];
	fast_line_intersect(flags, clipped[second_index].p[inside_index[1]], (clipped[second_index].p) + outside_index);
	t_vec3	output = clipped[second_index].p[outside_index];
	//fast_line_intersect(flags, clipped[1].p[inside_index[1]], clipped[cur_index].p + outside_index);
	clipped[second_index].p[inside_index[0]] = clipped[cur_index].p[outside_index];
	for (int i = 0; i < 3; i++)
	{
		if (!flags[0] && !flags[1] && clipped[cur_index].p[i].x < 0.0f)
		{
			printf("case 3\n");
			printf("outside index: %d\n", outside_index);
			print_vec3(clipped[cur_index].p[0], "1: clipped y HEIGTH");
			print_vec3(clipped[cur_index].p[1], "1: clipped y HEIGTH");
			print_vec3(clipped[cur_index].p[2], "1: clipped y HEIGTH");
			print_vec3(clipped[1].p[0], "2: clipped y HEIGTH");
			print_vec3(clipped[1].p[1], "2: clipped y HEIGTH");
			print_vec3(clipped[1].p[2], "2: clipped y HEIGTH");
			break ;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (!flags[0] && !flags[1] && clipped[second_index].p[i].x < -0.0f)
		{
			printf("case 4\n");
			printf("outside index: %d\n", outside_index);
			printf("inside index 1: %d\n", inside_index[0]);
			printf("inside index[1]: %d\n", inside_index[1]);
			print_vec3(clipped[cur_index].p[0], "1: clipped y HEIGTH");
			print_vec3(clipped[cur_index].p[1], "1: clipped y HEIGTH");
			print_vec3(clipped[cur_index].p[2], "1: clipped y HEIGTH");
			print_vec3(clipped[second_index].p[0], "2[0]: clipped y HEIGTH");
			print_vec3(clipped[second_index].p[1], "2[1]: clipped y HEIGTH");
			print_vec3(clipped[second_index].p[2], "2[2]: clipped y HEIGTH");
			print_vec3(input, "input");
			print_vec3(output, "output");
			break ;
		}
	}
}

// moves the triangles to the start so there are no empty slots
// returns the count of elments
//(used_indexes becomes invalid afterwards)
int8_t	fix_clipped_arr(t_triangle clipped[30], t_index_usage used_indexes)
{
	int8_t	first_empty_index;
	int8_t	last_used_index;
	int8_t	count;

	first_empty_index = get_set_free_index(&used_indexes);
	last_used_index = get_unset_last_used_index(&used_indexes);
	if (last_used_index == -1)
		return (0);
	while (last_used_index > first_empty_index)
	{
		clipped[first_empty_index] = clipped[last_used_index];
		first_empty_index = get_set_free_index(&used_indexes);

		last_used_index = get_unset_last_used_index(&used_indexes);
		if (last_used_index == -1)
		{
			fprintf(stderr, "err fix_clipped_arr\n");
			exit(1);
			return (0);
		}
	}
	count = first_empty_index + 1;
	for(int i = 0; i < count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (clipped[i].p[j].x <= 0.0f && clipped[i].p[j].x >= -0.0001f )
			{
				clipped[i].p[j].x = 0.0f;
				// printf("count: %d i: %d\n", count, i);
				// print_vec3(clipped[i].p[0],0);
				// print_vec3(clipped[i].p[1],0);
				// print_vec3(clipped[i].p[2],0);
				// exit(0);
			}
			if (clipped[i].p[j].y <= 0.0f && clipped[i].p[j].y >= -0.0001f )
			{
				clipped[i].p[j].y = 0.0f;
				// printf("count: %d i: %d\n", count, i);
				// print_vec3(clipped[i].p[0],0);
				// print_vec3(clipped[i].p[1],0);
				// print_vec3(clipped[i].p[2],0);
				// exit(0);
			}
		}
	}
	return (count);
}

// returns the new count
int8_t	call_clipping_xy(t_triangle clipped[30])
{
	int8_t			clipping_flags[2];
	int8_t			cur_index;
	t_index_usage	used_indexes;
	t_index_usage	used_indexes_old;
	
	clipping_flags[0] = 0;
	clipping_flags[1] = 0;
	cur_index = 0;
	used_indexes = 1ULL;

	clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
	
	if (!used_indexes)
		return (0);
	clipping_flags[0] = 0;
	clipping_flags[1] = 1;
	used_indexes_old = used_indexes;
	cur_index = get_unset_used_index(&used_indexes_old);
	while (cur_index != -1)
	{
		clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		if (!used_indexes)
			return (0);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	clipping_flags[0] = 1;
	clipping_flags[1] = 0;
	used_indexes_old = used_indexes;
	cur_index = get_unset_used_index(&used_indexes_old);
	while (cur_index != -1)
	{
		clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		if (!used_indexes)
			return (0);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	clipping_flags[0] = 1;
	clipping_flags[1] = 1;
	used_indexes_old = used_indexes;
	cur_index = get_unset_used_index(&used_indexes_old);
	while (cur_index != -1)
	{
		clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		if (!used_indexes)
			return (0);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	return (fix_clipped_arr(clipped, used_indexes));
}
