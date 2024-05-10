/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/10 03:46:34 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

t_triangle *clipped_g;

t_triangle input;

int count;

void	ft_put_binary(int64_t nb)
{
	int	i;

	i = 10;
	while (i >= 0)
	{
		printf("%llu", ((1ULL << i) & nb) >> i);
		if (!(i % 4))
			printf(".");
		i--;
	}
	printf("\n");
}

// returns true if the elemnt in clipped[] is empty
bool	is_empty(t_triangle *clipped)
{
	if (clipped->normal.x == 0.0f && clipped->normal.y == 0.0f && clipped->normal.z == 0.0f)
		return (true);
	return (false);
}

int8_t	get_free_index(t_index_usage *const usage)
{
	int8_t		i;
	t_index_usage	local;

	i = 0;
	local = *usage;
	while (i < 64)
	{
		if ((local & (1ULL << i)) == 0)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

int8_t	get_unset_last_used_index(t_index_usage *const usage)
{
	int8_t			i;
	t_index_usage	local;

	i = 63;
	local = *usage;
	while (i >= 0)
	{
		if (local & (1ULL << i))
		{
			*usage ^= 1ULL << i;
			return (i);
		}
		i--;
	}
	return (-1);
}

int8_t	get_unset_used_index(t_index_usage *usage)
{
	int8_t		i;

	i = 0;
	while (i < 64)
	{
		if (((*usage) & (1ULL << i)))
		{
			*usage &= ~(1ULL << i);
			return (i);
		}
		i++;
	}
	return (-1);
}

void	set_index_usage(t_index_usage *usage, int8_t index, int8_t state)
{
	if (state)
		*usage |= (1ULL << index);
	else
	{
		//clipped_g[index].p[0].x = 222222;
		*usage &= ~(1ULL << index);
	}
}

bool	index_is_used(t_index_usage usage, int index)
{
	return(usage & (1ULL << index));
}

void fast_line_intersect(const int8_t base_flags[2], t_vec3 p1, t_vec3 *p2)
{
	float progress;
	float dist_x = p2->x - p1.x;
	float dist_y = p2->y - p1.y;

	if ((zero_f(dist_x) && !base_flags[1]) || (zero_f(dist_y) && base_flags[1]))
	{
		print_vec3(p1, "p1: ");
		print_vec3(*p2, "p2:");
		exit(0);
	}

	if (base_flags[0])
	{
		if (base_flags[1])
			progress = (HEIGHT - 1.0f - p1.y) / dist_y;
		else
			progress = (WIDTH - 1.0f - p1.x) / dist_x;
	}
	else
	{
		if (base_flags[1])
			progress = -p1.y / dist_y;
		else
			progress = -p1.x / dist_x;
	}
	p2->x = p1.x + dist_x * progress;
	p2->y = p1.y + dist_y * progress;

	if (p2->x < 0)
		p2->x = 0;
	else if (p2->x >= WIDTH)
		p2->x = WIDTH - 1;
	if (p2->y < 0)
		p2->y = 0;
	else if (p2->y >= HEIGHT)
		p2->y = HEIGHT - 1;

	p2->u = p1.u + (p2->u - p1.u) * progress;
	p2->v = p1.v + (p2->v - p1.v) * progress;
}

// /*
// 	if base_flags[0] -> l1 goes through (WIDTH/HEIGHT)
// 	if !base_flags[0] -> l1 goes through (0, 0)
// 	if base_flags[1] -> l1 is along the y axis
// 	if !base_flags[1] -> l1 is along the x axis
// */
// void	fast_line_intersect(const int8_t base_flags[2], t_vec3 p1, t_vec3 *p2)
// {
// 	float	progress;
// 	float	dist_x;
// 	float	dist_y;

// 	dist_x = ((p2->x) - (p1.x));
// 	dist_y = ((p2->y) - (p1.y));
// 	if ((zero_f(dist_x) && !base_flags[1]) || (zero_f(dist_y) && base_flags[1]))
// 	{
// 		print_vec3(p1, "p1: ");
// 		print_vec3(*p2, "p2:");
// 		exit(0);
// 	}
// 	if (base_flags[0])
// 	{
// 		if (base_flags[1])
// 			progress = (((HEIGHT - 1.0) - (int)(p1.y)) / (dist_y));
// 		else
// 			progress = (((WIDTH  - 1.0) - (int)(p1.x)) / (dist_x));
// 	}
// 	else
// 	{
// 		if (base_flags[1])
// 			progress = ((-(int)(p1.y)) / (dist_y));
// 		else
// 			progress = ((-(int)(p1.x)) / (dist_x));
// 	}
// 	p2->x = p1.x + (dist_x) * progress;
// 	p2->y = p1.y + (dist_y) * progress;
// 	if (p2->x > 10)
// 		p2->x -= 1.0f;
// 	else
// 		p2->x += 1.0f;
// 	if (p2->y > 10)
// 		p2->y -= 1.0f;
// 	else
// 		p2->y += 1.0f;
// 	// if (p2->y > (float)HEIGHT && p2->y - 1 < (float)HEIGHT)
// 	// 	p2->y =
// 	// if (p2->x < 0.0f && fabs(p2->x) > 0.01)
// 	// 	p2->x = 0.0f;
// 	// if (p2->y < 0.0f && p2->y > -0.01f)
// 	// 	p2->y = 0.0f;
// 	// if (p2->x < 0.0f && p2->x > -0.01f)
// 	// 	p2->x = 0.0f;
// 	p2->y = (p2->y);
// 	p2->x = (p2->x);
// 	p2->u = (p2->u - p1.u) * progress;
// 	p2->v = (p2->v - p1.v) * progress;
// 	// TODO INACCUARATE UV VALS CAUSE SEGFAULT WITH LOTS OF CLIPPING
// 	//TODO keep in mind when texture mapping, these val might be fked since p2->x/y are fked
// }

void	clipping_xy(t_triangle *clipped, t_index_usage *used_indexes, const int8_t flags[2], int8_t cur_index)
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
		if (!flags[0] && !flags[1] && clipped[cur_index].p[i].x >= 0.0f)
			inside_index[inside_points++] = i;
		else if (!flags[0] && flags[1] && clipped[cur_index].p[i].y >= 0.0f)
			inside_index[inside_points++] = i;
		else if (flags[0] && !flags[1] && clipped[cur_index].p[i].x < (float)WIDTH)
			inside_index[inside_points++] = i;
		else if (flags[0] && flags[1] &&clipped[cur_index].p[i].y < (float)HEIGHT)
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
		count--;
		return ;
	}
	if (inside_points == 3)
	{
		//set_index_usage(used_indexes, cur_index, 1);
		// if (flags[0] && flags[1])
		// {
		// 	for (int i = 0; i < 3; i++)
		// 	{
		// 		if (!flags[0] && !flags[1] && clipped[cur_index].p[i].x < 0.0f)
		// 		{
		// 			printf("case 1\n");
		// 			print_vec3(clipped[cur_index].p[0], "clipped y HEIGTH");
		// 			print_vec3(clipped[cur_index].p[1], "clipped y HEIGTH");
		// 			print_vec3(clipped[cur_index].p[2], "clipped y HEIGTH");
		// 			break ;
		// 		}
		// 	}
		// }
		return ;
	}
	if (inside_points == 1)
	{
		//printf("here\n");
		set_index_usage(used_indexes, cur_index, 1);
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		fast_line_intersect(flags, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index1);
		fast_line_intersect(flags, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index2);
		// for (int i = 0; i < 3; i++)
		// {
		// 	if (flags[0] && flags[1] && clipped[cur_index].p[i].y >= HEIGHT)
		// 	{
		// 		printf("case 2\n");
		// 		print_vec3(clipped[cur_index].p[0], "clipped y HEIGTH");
		// 		print_vec3(clipped[cur_index].p[1], "clipped y HEIGTH");
		// 		print_vec3(clipped[cur_index].p[2], "clipped y HEIGTH");
		// 		exit(1);
		// 		break ;
		// 	}
		// }
		clipped[cur_index].col = RED;
		return ;
	}
	count++;
	int8_t second_index;
	set_index_usage(used_indexes, cur_index, 1);
	second_index = get_free_index(used_indexes);
	set_index_usage(used_indexes, second_index, true);
	clipped[second_index] = clipped[cur_index];
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	fast_line_intersect(flags, clipped[cur_index].p[inside_index[0]], (clipped[cur_index].p) + outside_index);
	t_vec3 input_fn_left = clipped[second_index].p[inside_index[1]];
	t_vec3 input_fn_right = clipped[second_index].p[outside_index];
	fast_line_intersect(flags, clipped[second_index].p[inside_index[1]], (clipped[second_index].p) + outside_index);
	t_vec3	output_fn = clipped[second_index].p[outside_index];
	//fast_line_intersect(flags, clipped[1].p[inside_index[1]], clipped[cur_index].p + outside_index);
	clipped[second_index].p[inside_index[0]] = clipped[cur_index].p[outside_index];
	clipped[cur_index].col = PINK;
	clipped[cur_index].col = DARK_GREY;
	// for (int i = 0; i < 3; i++)
	// {
	// 	if (flags[0] && flags[1] && clipped[second_index].p[i].y >= HEIGHT)
	// 	{
	// 		printf("case 3\n");
	// 		printf("outside index: %d\n", outside_index);
	// 		print_vec3(input.p[0], "input");
	// 		print_vec3(input.p[1], "input");
	// 		print_vec3(input.p[2], "input");
	// 		print_vec3(clipped[cur_index].p[0], "1: clipped y HEIGTH");
	// 		print_vec3(clipped[cur_index].p[1], "1: clipped y HEIGTH");
	// 		print_vec3(clipped[cur_index].p[2], "1: clipped y HEIGTH");
	// 		print_vec3(clipped[1].p[0], "2: clipped y HEIGTH");
	// 		print_vec3(clipped[1].p[1], "2: clipped y HEIGTH");
	// 		print_vec3(clipped[1].p[2], "2: clipped y HEIGTH");
	// 		exit(0);
	// 		break ;
	// 	}
	// }
	// for (int i = 0; i < 3; i++)
	// {
	// 	if (flags[0] && flags[1] && clipped[cur_index].p[i].y >= HEIGHT)
	// 	{
	// 		printf("case 4\n");
	// 		printf("outside index: %d\n", outside_index);
	// 		printf("inside index 1: %d\n", inside_index[0]);
	// 		printf("inside index[1]: %d\n", inside_index[1]);
	// 		print_vec3(input.p[0], "input");
	// 		print_vec3(input.p[1], "input");
	// 		print_vec3(input.p[2], "input");
	// 		print_vec3(clipped[cur_index].p[0], "1: clipped y HEIGTH");
	// 		print_vec3(clipped[cur_index].p[1], "1: clipped y HEIGTH");
	// 		print_vec3(clipped[cur_index].p[2], "1: clipped y HEIGTH");
	// 		print_vec3(clipped[second_index].p[0], "2[0]: clipped y HEIGTH");
	// 		print_vec3(clipped[second_index].p[1], "2[1]: clipped y HEIGTH");
	// 		print_vec3(clipped[second_index].p[2], "2[2]: clipped y HEIGTH");
	// 		print_vec3(input_fn_left, "input_fn_left");
	// 		print_vec3(input_fn_right, "input_fn_right");
	// 		print_vec3(output_fn, "output of fn");
	// 		exit(0);
	// 		break ;
	// 	}
	// }
}

// moves the triangles to the start so there are no empty slots
// returns the count of elments
//(used_indexes becomes invalid afterwards)
int8_t	fix_clipped_arr(t_triangle *clipped, t_index_usage used_indexes)
{
	int8_t	first_empty_index;
	int8_t	last_used_index;
	//int8_t	count;

	//printf("-----------\n");
	int		swap_count = 0;
	if (!used_indexes)
		return (0);
	first_empty_index = get_free_index(&used_indexes);
	last_used_index = get_unset_last_used_index(&used_indexes);
	if (last_used_index == -1)
		return (0);
	//printf("first_empty_index: %d\nlast_used_index: %d\n", first_empty_index, last_used_index);
	if (last_used_index == first_empty_index)
	{
		fprintf(stderr, "error fix_clipped_arr: both indexes: %d (before loop)\n", last_used_index);
		exit(1);
	}
	if (last_used_index == -1)
		return (0);
	while (last_used_index > first_empty_index)
	{
		if (last_used_index == first_empty_index)
		{
			fprintf(stderr, "error fix_clipped_arr: both indexes: %d (in loop)\n", last_used_index);
			exit(1);
		}
		clipped[first_empty_index] = clipped[last_used_index];
		set_index_usage(&used_indexes, first_empty_index, true);
		swap_count++;
		//clipped[last_used_index].p[0].x = 11111;
		first_empty_index = get_free_index(&used_indexes);
		last_used_index = get_unset_last_used_index(&used_indexes);
		//printf("first_empty_index: %d\nlast_used_index: %d\n", first_empty_index, last_used_index);
		
		if (last_used_index == -1)
		{
			fprintf(stderr, "err fix_clipped_arr\n");
			exit(1);
			return (0);
		}
	}
	set_index_usage(&used_indexes, last_used_index, true);
	if (last_used_index == first_empty_index)
	{
		fprintf(stderr, "error fix_clipped_arr: both indexes: %d(after loop)\n", last_used_index);
		exit(1);
	}
	if (count != first_empty_index)
	{
		fprintf(stderr, "Wrong count in fixed_clipped_arr, expeceted: %d actual: %d\n", count, first_empty_index +1);
		exit(1);
	}
	for (int i = 0; i < count; i++)
	{
		for (int d = 0; d < 3; d++)
		{
			// if (clipped[i].p[d].x < 0.0f || )
			if (clipped[i].p[d].x < 0.0f || clipped[i].p[d].y < 0.0f || clipped[i].p[d].x >= WIDTH  || clipped[i].p[d].y >= HEIGHT)
			{
				printf("err: found invalid vector while shifiting array to start\n");
				printf("count: %d, i: %d\n", count, i);
				printf("swap count: %d\n", swap_count);
				print_vec3(input.p[0], "input");
				print_vec3(input.p[1], "input");
				print_vec3(input.p[2], "input");
				ft_put_binary(used_indexes);
				print_vec3(clipped[i].p[0], "0");
				print_vec3(clipped[i].p[1], "1");
				print_vec3(clipped[i].p[2], "2");

				printf("\n\n");
				for (int q = 0; q < 5; q++)
				{
					printf("tri %d:\n", q);
					print_vec3(clipped[q].p[0], "0");
					print_vec3(clipped[q].p[1], "1");
					print_vec3(clipped[q].p[2], "2");
				}
				exit (1);
				break;
			}
		}

	}
	// for(int i = 0; i < count; i++)
	// {
	// 	for (int j = 0; j < 3; j++)
	// 	{
	// 		if (clipped[i].p[j].x <= 0.0f && clipped[i].p[j].x >= -0.0001f )
	// 		{
	// 			clipped[i].p[j].x = 0.0f;
	// 			// printf("count: %d i: %d\n", count, i);
	// 			// print_vec3(clipped[i].p[0],0);
	// 			// print_vec3(clipped[i].p[1],0);
	// 			// print_vec3(clipped[i].p[2],0);
	// 			// exit(0);
	// 		}
	// 		if (clipped[i].p[j].y <= 0.0f && clipped[i].p[j].y >= -0.0001f )
	// 		{
	// 			clipped[i].p[j].y = 0.0f;
	// 			// printf("count: %d i: %d\n", count, i);
	// 			// print_vec3(clipped[i].p[0],0);
	// 			// print_vec3(clipped[i].p[1],0);
	// 			// print_vec3(clipped[i].p[2],0);
	// 			// exit(0);
	// 		}
	// 	}
	// }
	return (count);
}

// returns the new count
int8_t	call_clipping_xy(t_triangle *clipped)
{
	int8_t			clipping_flags[2];
	int8_t			cur_index;
	t_index_usage	used_indexes;
	t_index_usage	used_indexes_old;

input = clipped[0];
	clipped_g = clipped;
	clipping_flags[0] = 0;
	clipping_flags[1] = 0;
	cur_index = 0;
	used_indexes = 1ULL;
	count = 1;
	// printf("before:\n");
	// 	print_vec3(clipped[0].p[0], "0");
	// 	print_vec3(clipped[0].p[1], "1");
	// 	print_vec3(clipped[0].p[2], "2");
	clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
	//return (fix_clipped_arr(clipped, used_indexes));
	// printf("used_indexes:\n");
	// ft_put_binary(used_indexes);
	// for (int q = 0; q < 5; q++)
	// {
	// 	if (index_is_used(used_indexes, q))
	// 	{
	// 		printf("tri %d:\n", q);
	// 		print_vec3(clipped[q].p[0], "0");
	// 		print_vec3(clipped[q].p[1], "1");
	// 		print_vec3(clipped[q].p[2], "2");
	// 	}
	// }
	if (!used_indexes)
		return (0);
	clipping_flags[0] = 0;
	clipping_flags[1] = 1;
	used_indexes_old = used_indexes;
	// printf("used_indexes:\n");
	// ft_put_binary(used_indexes);
	// printf("used_indexes_old:\n");
	// ft_put_binary(used_indexes_old);
	cur_index = get_unset_used_index(&used_indexes_old);
	//while (cur_index != -1)
	int old_count = count;
	for (int i =0; i < old_count; i++)
	{
		if (cur_index == -1)
		{
			fprintf(stderr, "cur_index == -1");
			exit(1);
		}
		clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		if (!used_indexes)
			return (0);
		// printf("used_indexes:\n");
		// ft_put_binary(used_indexes);
		// printf("used_indexes_old:\n");
		// ft_put_binary(used_indexes_old);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	// 	printf("used_indexes:\n");
	// 	ft_put_binary(used_indexes);
	// 	printf("used_indexes_old:\n");
	// 	ft_put_binary(used_indexes_old);
	// printf("after clipping x0 y0:\n");
	// for (int q = 0; q < 5; q++)
	// {
	// 	//if (index_is_used(used_indexes, q))
	// 	{
	// 		printf("tri %d:\n", q);
	// 		print_vec3(clipped[q].p[0], "0");
	// 		print_vec3(clipped[q].p[1], "1");
	// 		print_vec3(clipped[q].p[2], "2");
	// 	}
	// }
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
