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
// 		int	y_index = y * WIDTH;
// 		for (int x = min[X]; x <= max[X]; x++)
// 		{
// 			cur[X] = x;
// 			if (edge_fn(a, b, cur) && edge_fn(a, c, cur) && edge_fn(b, c, cur))
// 			{
// 				//fprintf(stderr, "x: %d y: %d\n", x, y);
// 				((uint32_t *)img->pixels)[x + y_index] = color;
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
	//double	m1 = slope_2d_x_per_y(p[0], p[1]);
	double	y_dist1 = p[1].y - p[0].y;
	double	cur_y_lf = p[0].y;
	double	total_y_progress;
	int		cur_x;
	static int i = 0;
	int y_index =  (int)roundf(cur_y_lf);
	while (cur_y_lf <= p[1].y && y_index < HEIGHT)
	{
		total_y_progress = (cur_y_lf - p[0].y) / (p[2].y - p[0].y);
		cur_x = (int)roundf((p[2].x - p[0].x) * total_y_progress + p[0].x);
		double	y_progress =  (cur_y_lf - p[0].y) / (p[1].y - p[0].y);
		int	x_max = (int)roundf((p[1].x - p[0].x) * y_progress + p[0].x);
		double cur_z;
		double start_z = y_progress * (projected->unprojected_z[1] - projected->unprojected_z[0]) + projected->unprojected_z[0];
		double end_z = total_y_progress * (projected->unprojected_z[2] - projected->unprojected_z[0]) + projected->unprojected_z[0];
		int	len_x = x_max - cur_x;
		double	start_x = cur_x;
		double	z_dist = end_z - start_z;
		int row_index = WIDTH * y_index;
		if (cur_x < x_max)
		{
			if (cur_x < 0)
				cur_x = 0;
			while (cur_x <= x_max && cur_x < WIDTH)
			{
				double x_progress = fabs(cur_x - start_x) / len_x;
				cur_z = x_progress * z_dist + start_z;
				int fin_index = cur_x + row_index;
				if (cur_z > Z_NEAR && cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				cur_x++;
			}
		}
		else if (cur_x > x_max)
		{
			if (cur_x >= WIDTH)
				cur_x = WIDTH - 1;
			while (cur_x >= x_max && cur_x >= 0)
			{
				double x_progress = (cur_x - start_x) / len_x;
				cur_z = x_progress * z_dist + start_z;
				int fin_index = cur_x + row_index;
				if (cur_z > Z_NEAR && cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				cur_x--;
			}
		}
		cur_y_lf = cur_y_lf + 1.0f;
		y_index =  (int)roundf(cur_y_lf);
	}
	double	m2 = slope_2d_x_per_y(p[0], p[2]);
	double	m3 = slope_2d_x_per_y(p[1], p[2]);
	int x_max;
	cur_y_lf = p[1].y;
	if (cur_y_lf < 0.0f)
	{
		cur_y_lf = 0.0f;
	}
	y_index = (int)roundf(cur_y_lf);
	while (cur_y_lf <= p[2].y && y_index < HEIGHT)
	{
		double	y_progress =  (cur_y_lf - p[1].y) / (p[2].y - p[1].y);
		total_y_progress = (cur_y_lf - p[0].y) / (p[2].y - p[0].y);
		cur_x = (int)roundf((m3 * (cur_y_lf - p[1].y) + p[1].x));
		x_max =  (int)roundf(((m2 * (cur_y_lf - p[2].y) + p[2].x)));
		double cur_z;
		double start_z = y_progress * (projected->unprojected_z[2] - projected->unprojected_z[1]) + projected->unprojected_z[1];
		double end_z = y_progress * (projected->unprojected_z[2] - projected->unprojected_z[0]) + projected->unprojected_z[0];
		int	len_x = x_max - cur_x;
		double	start_x = cur_x;
		double	z_dist = end_z - start_z;
		int row_index = WIDTH * y_index;
		if (cur_x < x_max)
		{
			if (cur_x < 0)
				cur_x = 0;
			while(cur_x <= x_max && cur_x < WIDTH)
			{
				double x_progress = (cur_x - start_x) / len_x;
				cur_z = x_progress * z_dist + start_z;
				int fin_index = cur_x + row_index;
				if (cur_z > Z_NEAR && cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				cur_x++;
			}
		}
		else if (cur_x > x_max)
		{
			if (cur_x >= WIDTH)
				cur_x = WIDTH - 1;
			while (cur_x >= x_max && cur_x >= 0)
			{
				double x_progress = (cur_x - start_x) / len_x;
				cur_z = x_progress * z_dist + start_z;
				int fin_index = cur_x + row_index;
				if (cur_z > Z_NEAR && cur_z < depth[fin_index])
				{
					depth[fin_index] = cur_z;
					pixels[fin_index] = color;
				}
				cur_x--;
			}
		}
		cur_y_lf += 1.0f;
		y_index =  (int)roundf(cur_y_lf);
	}
}
