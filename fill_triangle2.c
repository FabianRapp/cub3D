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

uint32_t colors[] = {
0xFF000001,
0xFF000100,
0xFF00FF01,
0xFFFF0001,
0xFFFF00FF,
0xFF00FFFF,
0xFFFFFFFF,
0xFF00FFFF,
0xFFFF00A1,
0xFF0080FF,
0xFF808001,
0xFFFF80FF,
0xFF80FF01,
0xFFD3D3D4,
0xFFA9A9A9,
};

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

void	fill_triangle_color(mlx_image_t *img, t_triangle *projected, uint32_t color, t_mesh *mesh)
{
	t_vec3	*p = projected->p;
	double		*depth;
	uint32_t	*pixels = (uint32_t *)img->pixels;
	//static int color_index = 0;
	//color = colors[color_index++];
	//if (color_index >= sizeof colors / sizeof(uint32_t))
	//	color_index = 0;
	depth = mesh->main->depth;
	sort_vertexes_for_y(projected);
	assume(p[0].y <= p[1].y && p[1].y <= p[2].y);
	for (int i = 0; i < 3; i++)
	{
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

	double	z_dist10 = p[1].z - p[0].z;
	double	z_dist20 = p[2].z- p[0].z;

	double	cur_y_lf = p[0].y;
	double	total_y_progress = 0;
	int		cur_col;
	int row_index =  (int)round(cur_y_lf);
	double section_y_progress = 0;
	if (!zero_f(y_dist10))
	{
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
			first_col_z = total_y_progress * z_dist20 + p[0].z;
			last_col_z = section_y_progress * z_dist10 + p[0].z;
			int	cur_col = first_col;
			int row_start_offset = WIDTH * row_index;
			int	direct_x;
			if (first_col <= last_col)
				direct_x = 1;
			else
				direct_x = -1;
			int	len_x = last_col - first_col + direct_x;
			double	z_dist = last_col_z - first_col_z;//+ direct_x;
			assume(len_x);
			for (int i = 0; i < abs(len_x); i++)
			{
				//double p = cur_col - first_col;
				//cur_z = (p * z_dist) / ((double)len_x) + first_col_z;
double t = (cur_col - first_col) / (double)len_x;
cur_z = first_col_z + t * (last_col_z - first_col_z);
				assume(cur_col < WIDTH);
				double x_progress;
				x_progress = 1.0 - ((last_col - cur_col) / (double)len_x);
				int fin_index = cur_col + row_start_offset;
				if (cur_z < depth[fin_index])
				{
					//printf("%lf\n", cur_z);
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				cur_col += direct_x;
			}
			cur_y_lf = cur_y_lf + 1.0f;
			row_index =  (int)round(cur_y_lf);
		}
	}
	double	y_dist21 = p[2].y - p[1].y;
	double	x_dist21 = p[2].x - p[1].x;
	if (zero_f(y_dist21))
		return ;
	double	z_dist21 = p[2].z - p[1].z;
	int last_col;
	cur_y_lf = p[1].y;
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
		assume(section_y_progress <= total_y_progress);
		int first_col = (int)round(x_dist20 * total_y_progress + p[0].x);
		last_col = (int)round(p[1].x + section_y_progress * x_dist21);
		double cur_z;
		double first_col_z = total_y_progress * z_dist20 + p[0].z;
		double last_col_z = section_y_progress * z_dist21 + p[1].z;
		cur_col = first_col;
		double	z_dist = last_col_z - first_col_z;
		int row_start_offset = WIDTH * row_index;
		assume(row_start_offset + WIDTH <= WIDTH * HEIGHT);
		int	direct_x;
		if (first_col <= last_col)
			direct_x = 1;
		else
			direct_x = -1;
		int	len_x = last_col - first_col + direct_x;
		assume(first_col >= 0 && first_col < WIDTH);
		assume(last_col >= 0 && last_col < WIDTH);
		for (int i = 0; i < abs(len_x); i++)
		{
			//double p = cur_col - first_col;
			//last_col_z - first_col_z;
			//cur_z = (p * z_dist) / ((double)len_x) + first_col_z;
double t = (cur_col - first_col) / (double)len_x;
cur_z = first_col_z + t * (last_col_z - first_col_z);
			int fin_index = cur_col + row_start_offset;
			if (cur_z < depth[fin_index])
			{
				depth[fin_index] = cur_z;
				pixels[fin_index] = color;
			}
			if (first_col == last_col)
				return ;
			cur_col += direct_x;
		}
		cur_y_lf += 1.0f;
		row_index =  (int)round(cur_y_lf);
	}
}

