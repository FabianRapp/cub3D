/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 05:11:27 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"


int8_t	edge_fn(int a[2], int b[2], int c[2])
{
	return (((b[X] - a[X]) * (c[Y] - a[Y]) * (b[Y] - a[Y]) * (c[X] - a[X])) > 0);
}


void	fill_min_max_xy(int a[2], int b[2], int c[2], int min[2], int max[2])
{
	min[X] = 0;
	if (a[X] > 0)
		min[X] = a[X];
	if (b[X] < min[X] && b[X] > 0)
		min[X] = b[X];
	if (c[X] < min[X] && c[X] > 0)
		min[X] = c[X];
	min[Y] = 0;
	if (a[Y] > 0)
		min[Y] = a[Y];
	if (b[Y] < min[Y] && b[Y] > 0)
		min[Y] = b[Y];
	if (c[Y] < min[Y] && c[Y] > 0)
		min[Y] = c[Y];

	max[X] = WIDTH - 1;
	if (a[X] < WIDTH)
		max[X] = a[X];
	if (b[X] > max[X] && b[X] < WIDTH)
		max[X] = b[X];
	if (c[X] > max[X] && b[X] < WIDTH)
		max[X] = c[X];
	max[Y] = HEIGHT - 1;
	if (a[Y] < HEIGHT)
		max[Y] = a[Y];
	if (b[Y] > max[Y] && b[Y] < HEIGHT)
		max[Y] = b[Y];
	if (c[Y] > max[Y] && c[Y] < HEIGHT)
		max[Y] = c[Y];
}

// void	fill_triangle_color(mlx_image_t *img, t_triangle *projected, uint32_t color, t_mesh *mesh)
// {
// 	int	a[2];
// 	int	b[2];
// 	int	c[2];
// 	int	cur[2];
// 	int	min_x;

// 	a[X] = (int)projected->p[0].x;
// 	a[Y] = (int)projected->p[0].y;
// 	b[X] = (int)projected->p[1].x;
// 	b[Y] = (int)projected->p[1].y;
// 	c[X] = (int)projected->p[2].x;
// 	c[Y] = (int)projected->p[2].y;

// 	int	min[2];
// 	int	max[2];

// 	min[X] = 0;
// 	min[Y] = 0;


// 	fill_min_max_xy(a, b, c, min, max);
// 	// max[X] = WIDTH - 1;
// 	// max[Y] = HEIGHT - 1;
// 	for (int y = min[Y]; y <= max[Y]; y++)
// 	{
// 		cur[Y] = y;
// 		int	row_index = y * WIDTH;
// 		for (int x = min[X]; x <= max[X]; x++)
// 		{
// 			cur[X] = x;
// 			if (edge_fn(a, b, cur) && edge_fn(a, c, cur) && edge_fn(b, c, cur))
// 			{
// 				//fprintf(stderr, "x: %d y: %d\n", x, y);
// 				((uint32_t *)img->pixels)[x + row_index] = color;
// 			}
// 			//fprintf(stderr, "index: %d (max: %d)\n", x + y * WIDTH, WIDTH * HEIGHT);
// 		}
// 	}
// }

void	fill_triangle_color(mlx_image_t *img, t_triangle *projected, uint32_t color, t_mesh *mesh)
{
	t_vec3	*p = projected->p;
	double		*depth;
	uint32_t	*pixels = (uint32_t *)img->pixels;

	//fprintf(stderr, "here\n");
	depth = mesh->main->depth;
	sort_vertexes_for_y(projected);
	if (!(p[0].y <= p[1].y && p[1].y <= p[2].y))
	{
		fprintf(stderr, "error sort_vertexes_for_y 1: %f 2: %f 3: %f\n", p[0].y, p[1].y, p[2].y);
		exit(1);
	}
	for (int i = 0; i < 3; i++)
	{
		if ((int)round(p[i].x) < 0)
			printf("x: lf: %lf round int: %d\n", p[i].x, (int)round(p[i].x));
		if ((int)round(p[i].x) >= WIDTH)
			printf("x: lf: %lf round int: %d (width: %d)\n", p[i].x, (int)round(p[i].x), WIDTH);
		if ((int)round(p[i].y) < 0)
			printf("y: lf: %lf round int: %d\n", p[i].y, (int)round(p[i].y));
		if ((int)round(p[i].y) >=HEIGHT)
			printf("y: lf: %lf round int: %dheight: %d\n", p[i].y, (int)round(p[i].y), HEIGHT);
		assume((int)round(p[i].x) >= 0);
		assume((int)round(p[i].x) < WIDTH);
		assume((int)round(p[i].y) >= 0.0);
		assume((int)round(p[i].y) < HEIGHT);
	}
	//double	m1 = slope_2d_x_per_y(p[0], p[1]);
	double	y_dist10 = p[1].y - p[0].y;
	double	y_dist20 = p[2].y - p[0].y;
	if (zero_f(y_dist20))
		return ;
	double	x_dist10 = p[1].x - p[0].x;
	double	x_dist20 = p[2].x - p[0].x;

	double	unprojected_z_dist10 = p[1].unprojected_z - p[0].unprojected_z;
	double	unprojected_z_dist20 = p[2].unprojected_z - p[0].unprojected_z;

	t_vec3	split_vertex;

	split_vertex.y = p[1].y;

	double	split_progress = y_dist10 / y_dist20;

	//split_vertex.x = ;
	//split_vertex.unprojected_z = ;

	double	cur_y_lf = p[0].y;
	double	total_y_progress = 0;
	int		col_index;
	static int i = 0;
	int row_index =  (int)round(cur_y_lf);
	double section_y_progress = 0;
	if (!zero_f(y_dist10))
	{
		//while (cur_y_lf <= p[1].y)
		while (section_y_progress < 1.0)
		{
			if (row_index >= HEIGHT)
				return ;
			total_y_progress = (cur_y_lf - p[0].y) / y_dist20;
			if (total_y_progress >= 1.0)
				return ;
			assume(total_y_progress >= 0.0 && total_y_progress <= 1.0);
			int first_col = (int)round(x_dist20 * total_y_progress + p[0].x);
			section_y_progress =  (cur_y_lf - p[0].y) / y_dist10;
			if (section_y_progress >= 1.0)
				break ;
			int	last_col = (int)round(x_dist10 * section_y_progress + p[0].x);
			double cur_z;
			double	first_col_z;
			double	last_col_z;
			first_col_z = total_y_progress * unprojected_z_dist10 + p[0].unprojected_z;
			last_col_z = section_y_progress * unprojected_z_dist20 + p[0].unprojected_z;
			double	z_dist = last_col_z - first_col_z;
			int	len_x = last_col - first_col + 1;
			if (!len_x)
			{
				cur_y_lf = cur_y_lf + 1.0f;
				row_index =  (int)round(cur_y_lf);
				continue ;
			}
			int	col_index = first_col;
			int row_start_offset = WIDTH * row_index;
			int	direct_x;
			if (first_col <= last_col)
				direct_x = 1;
			else
				direct_x = -1;
			while (1)
			{
				assume(col_index < WIDTH);
				double x_progress;
				//if (col_index == first_col)
				//{
				//	x_progress = 0.0;
				//	cur_z = first_col_z;
				//}
				//else if (col_index == last_col)
				//{
				//	x_progress = 1.0;
				//	cur_z = last_col_z;
				//}
				//else
				{
					x_progress = 1.0 - ((last_col - col_index) / (double)len_x);
					//x_progress = ((double)(col_index - first_col + 1)) / len_x;
					cur_z = x_progress * z_dist + first_col_z;
				}
				int fin_index = col_index + row_start_offset;
				assume(cur_z >= Z_NEAR && cur_z < Z_FAR);
				if (cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				if (abs(col_index - last_col) == 0)
					break ;
				col_index += direct_x;
			}
			cur_y_lf = cur_y_lf + 1.0f;
			row_index =  (int)round(cur_y_lf);
		}
	}
	double	y_dist21 = p[2].y - p[1].y;
	double	x_dist21 = p[2].x - p[1].x;
	if (zero_f(y_dist21))
		return ;
	double	unprojected_z_dist21 = p[2].unprojected_z - p[1].unprojected_z;
	double	m2 = slope_2d_x_per_y(p[0], p[2]);
	double	m3 = slope_2d_x_per_y(p[1], p[2]);
	long long int last_col;
	cur_y_lf = p[1].y;
	assert(cur_y_lf >= 0.0);
	row_index = (int)round(cur_y_lf);
	section_y_progress = 0.0;
	while (section_y_progress < 1.0)
	{
		if (row_index >= HEIGHT)
			return ;
		section_y_progress =  (cur_y_lf - p[1].y) / y_dist21;
		if (section_y_progress >= 1.0)
			return ;
		assume(section_y_progress >= 0.0 && section_y_progress <= 1.0);
		total_y_progress = (cur_y_lf - p[0].y) / y_dist20;
		if (total_y_progress >= 1.0)
			return ;
		assume(total_y_progress >= 0.0 && total_y_progress <= 1.0);
		assume(row_index >= 0);
		assume(row_index < HEIGHT);
		double	first_col = (int)round((m3 * (cur_y_lf - p[1].y) + p[1].x));
		last_col =  (long long)round(((m2 * (cur_y_lf - p[2].y) + p[2].x)));
		double cur_z;
		double first_col_z = section_y_progress * (unprojected_z_dist21) + p[1].unprojected_z;
		double last_col_z = section_y_progress * unprojected_z_dist20 + p[0].unprojected_z;
		int	len_x = last_col - first_col;
		col_index = first_col;
		double	z_dist = last_col_z - first_col_z;
		int row_start_offset = WIDTH * row_index;
		if (first_col < last_col)
		{
			if (last_col >= WIDTH)
				printf("last_col: %lld (width: %d)(line %d)\n", last_col, WIDTH, __LINE__);
			if (col_index < 0)
			{
				//printf("col_index < 0: %d in line %d\n", col_index, __LINE__);
				col_index = 0;
			}
			while(col_index <= last_col && col_index < WIDTH)
			{
				double x_progress = (col_index - first_col) / len_x;
				cur_z = x_progress * z_dist + first_col_z;
				int fin_index = col_index + row_start_offset;
				//if (cur_z < Z_NEAR)
				//	printf("cur_z: %lf (line %d, Z_NEAR: %f)\n", cur_z, __LINE__, Z_NEAR);
				if (cur_z > Z_NEAR && cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				col_index++;
			}
		}
		else if (first_col > last_col)
		{
			if (last_col < 0)
				printf("last_col: %lld (line %d)\n", last_col, __LINE__);
			if (col_index >= WIDTH)
			{
				printf("col_index >= WIDTH(%d): %d in line %d (last_col: %lld)\n", WIDTH, col_index, __LINE__, last_col);
				col_index = WIDTH - 1;
			}
			while (col_index >= last_col && col_index >= 0)
			{
				double x_progress = (col_index - first_col) / len_x;
				cur_z = x_progress * z_dist + first_col_z;
				int fin_index = col_index + row_start_offset;
				if (cur_z > Z_NEAR && cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				col_index--;
			}
		}
		cur_y_lf += 1.0f;
		row_index =  (int)round(cur_y_lf);
	}
}
