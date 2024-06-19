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
	static int color_index = 0;
	//color = colors[color_index++];
	if (color_index >= sizeof colors / sizeof(uint32_t))
		color_index = 0;
	const bool color_mode = 1;
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

	double	cur_y_lf = p[0].y;
	double	total_y_progress = 0;
	int		cur_col;
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
			first_col_z = total_y_progress * unprojected_z_dist20 + p[0].unprojected_z;
			last_col_z = section_y_progress * unprojected_z_dist10 + p[0].unprojected_z;
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
				double p = cur_col - first_col;
				//last_col_z - first_col_z;
				cur_z = (p * z_dist) / ((double)len_x) + first_col_z;
double t = (cur_col - first_col) / (double)len_x;
cur_z = first_col_z + t * (last_col_z - first_col_z);
				assume(cur_col < WIDTH);
				double x_progress;
				x_progress = 1.0 - ((last_col - cur_col) / (double)len_x);
				//x_progress = ((double)(cur_col - first_col + 1)) / len_x;
				//cur_z = x_progress * z_dist + first_col_z;
				int fin_index = cur_col + row_start_offset;
				if (cur_z < Z_NEAR)
					printf("%lf\n", cur_z);
				assume((cur_z >= Z_NEAR || zero_f(cur_z - Z_NEAR)) && cur_z < Z_FAR);
				if (cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					if (color_mode)
						pixels[fin_index] = color;
					else
					{
						if (direct_x == 1)
							pixels[fin_index] = GREEN;
						else
							pixels[fin_index] = RED;
					}
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
	double	unprojected_z_dist21 = p[2].unprojected_z - p[1].unprojected_z;
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
		double first_col_z = total_y_progress * unprojected_z_dist20 + p[0].unprojected_z;
		double last_col_z = section_y_progress * unprojected_z_dist21 + p[1].unprojected_z;
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
			double x_progress = 1.0 - (last_col - cur_col) / (double)len_x;
			double p = cur_col - first_col;
			//last_col_z - first_col_z;
			cur_z = (p * z_dist) / ((double)len_x) + first_col_z;
double t = (cur_col - first_col) / (double)len_x;
cur_z = first_col_z + t * (last_col_z - first_col_z);
			//double x_progress = (cur_col - first_col) / len_x;
			//cur_z = x_progress * z_dist + first_col_z;
			int fin_index = cur_col + row_start_offset;
			assume((cur_z >= Z_NEAR || zero_f(cur_z - Z_NEAR)) && cur_z < Z_FAR);
			if (cur_z < depth[fin_index])
			{
				depth[fin_index] = cur_z;
				if (color_mode)
					pixels[fin_index] = color;
				else
				{
					if (direct_x == 1)
						pixels[fin_index] = WHITE;
					else
						pixels[fin_index] = BLUE;
				}
			}
			if (first_col == last_col)
				return ;
			cur_col += direct_x;
		}
		cur_y_lf += 1.0f;
		row_index =  (int)round(cur_y_lf);
	}
}
