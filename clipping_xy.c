/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/09 01:14:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

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

int	clipping_xy(t_triangle *tri, t_triangle *clipped, const int8_t flags[2])
{
	int8_t				i;
	int8_t				inside_points;
	int8_t				inside_index[3];

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (!flags[0] && !flags[1] && tri->p[i].x > 0.0f)
			inside_index[inside_points++] = i;
		else if (!flags[0] && flags[1] && tri->p[i].y > 0.0f)
			inside_index[inside_points++] = i;
		else if (flags[0] && !flags[1] && tri->p[i].x < (float)WIDTH)
			inside_index[inside_points++] = i;
		else if (flags[0] && flags[1] && tri->p[i].y < HEIGHT)
		{
			// if (tri->p[i].y >= 699.00f && tri->p[i].y <= 701.00f)
			// 	printf("here\n");
			inside_index[inside_points++] = i;
		}
		i++;
	}
	if (inside_points == 0)
		return (0);
	if (inside_points == 3)
	{
		*clipped = *tri;
		if (flags[0] && flags[1])
		{
			for (int i = 0; i < 3; i++)
			{
				if (!flags[0] && !flags[1] && clipped[0].p[i].x < 0.0f)
				{
					printf("case 1\n");
					print_vec3(tri->p[0], "base y HEIGTH");
					print_vec3(tri->p[1], "base y HEIGTH");
					print_vec3(tri->p[2], "base y HEIGTH");
					print_vec3(clipped->p[0], "clipped y HEIGTH");
					print_vec3(clipped->p[1], "clipped y HEIGTH");
					print_vec3(clipped->p[2], "clipped y HEIGTH");
					break ;
				}
			}
		}
		return (1);
	}
	if (inside_points == 1)
	{
		clipped[0] = *tri;
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		fast_line_intersect(flags, tri->p[inside_index[0]], clipped->p + outside_index1);
		fast_line_intersect(flags, tri->p[inside_index[0]], clipped->p + outside_index2);
		for (int i = 0; i < 3; i++)
		{
			if (!flags[0] && !flags[1] && clipped[0].p[i].x < 0.0f)
			{
				printf("case 2\n");
				print_vec3(tri->p[0], "base y HEIGTH");
				print_vec3(tri->p[1], "base y HEIGTH");
				print_vec3(tri->p[2], "base y HEIGTH");
				print_vec3(clipped->p[0], "clipped y HEIGTH");
				print_vec3(clipped->p[1], "clipped y HEIGTH");
				print_vec3(clipped->p[2], "clipped y HEIGTH");
				break ;
			}
		}
		return (1);
	}
	clipped[0] = *tri;
	clipped[1] = *tri;
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	fast_line_intersect(flags, clipped[0].p[inside_index[0]], (clipped[0].p) + outside_index);
	t_vec3 input = clipped[1].p[inside_index[1]];
	fast_line_intersect(flags, clipped[1].p[inside_index[1]], (clipped[1].p) + outside_index);
	t_vec3	output = clipped[1].p[outside_index];
	//fast_line_intersect(flags, clipped[1].p[inside_index[1]], clipped[0].p + outside_index);
	clipped[1].p[inside_index[0]] = clipped[0].p[outside_index];
	for (int i = 0; i < 3; i++)
	{
		if (!flags[0] && !flags[1] && clipped[0].p[i].x < 0.0f)
		{
			printf("case 3\n");
			printf("outside index: %d\n", outside_index);
			print_vec3(tri->p[0], "base y HEIGTH");
			print_vec3(tri->p[1], "base y HEIGTH");
			print_vec3(tri->p[2], "base y HEIGTH");
			print_vec3(clipped->p[0], "1: clipped y HEIGTH");
			print_vec3(clipped->p[1], "1: clipped y HEIGTH");
			print_vec3(clipped->p[2], "1: clipped y HEIGTH");
			print_vec3(clipped[1].p[0], "2: clipped y HEIGTH");
			print_vec3(clipped[1].p[1], "2: clipped y HEIGTH");
			print_vec3(clipped[1].p[2], "2: clipped y HEIGTH");
			break ;
		}
	}
	for (int i = 0; i < 3; i++)
	{
		if (!flags[0] && !flags[1] && clipped[1].p[i].x < -0.0f)
		{
			printf("case 4\n");
			printf("outside index: %d\n", outside_index);
			printf("inside index 1: %d\n", inside_index[0]);
			printf("inside index[1]: %d\n", inside_index[1]);
			print_vec3(tri->p[0], "base y HEIGTH");
			print_vec3(tri->p[1], "base y HEIGTH");
			print_vec3(tri->p[2], "base y HEIGTH");
			print_vec3(clipped->p[0], "1[0]: clipped y HEIGTH");
			print_vec3(clipped->p[1], "1[1]: clipped y HEIGTH");
			print_vec3(clipped->p[2], "1[2]: clipped y HEIGTH");
			print_vec3(clipped[1].p[0], "2[0]: clipped y HEIGTH");
			print_vec3(clipped[1].p[1], "2[1]: clipped y HEIGTH");
			print_vec3(clipped[1].p[2], "2[2]: clipped y HEIGTH");
			print_vec3(input, "input");
			print_vec3(output, "output");
			break ;
		}
	}
	return (2);
}
