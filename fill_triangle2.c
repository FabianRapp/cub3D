/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 05:11:27 by frapp             #+#    #+#             */
/*   Updated: 2024/04/24 09:07:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>


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
// 				//printf("x: %d y: %d\n", x, y);
// 				((uint32_t *)img->pixels)[x + y_index] = color;
// 			}
// 			//printf("index: %d (max: %d)\n", x + y * WIDTH, WIDTH * HEIGHT);
// 		}
// 	}
// }

void	fill_triangle_color(mlx_image_t *img, t_triangle *projected, uint32_t color, t_mesh *mesh)
{
	t_vec3	*p = projected->p;
	float		*depth;
	uint32_t	*pixels = (uint32_t *)img->pixels;

	//printf("here\n");
	depth = mesh->main->depth;
	sort_vertexes_for_y(projected);
	if (!(p[0].y <= p[1].y && p[1].y <= p[2].y))
	{
		printf("error sort_vertexes_for_y 1: %f 2: %f 3: %f\n", p[0].y, p[1].y, p[2].y);
		exit(1);
	}
	//float	m1 = slope_2d_x_per_y(p[0], p[1]);
	float	y_dist1 = p[1].y - p[0].y;
	float	cur_y_float = p[0].y;
	float	total_y_progress;
	int		cur_x;
	static int i = 0;
	if (cur_y_float < 0.0f)
	{
		cur_y_float = 0.0f;
	}//todo: fix condtion for when objects leave the screen to the left and right
	//if ((p[0].x >= 0 || p[1].x >= 0) && (p[0].x < WIDTH || p[1].x < WIDTH) && (p[0].y >= 0 || p[1].y >= 0) && (p[0].y < HEIGHT || p[1].y < HEIGHT) && (p[0].z > Z_NEAR || p[1].z > Z_NEAR) && (projected->unprojected_z[0] < Z_FAR || projected->unprojected_z[1] < Z_FAR))
	{
		int y_index =  (int)roundf(cur_y_float);
		while (cur_y_float <= p[1].y && y_index < HEIGHT)
		{
			total_y_progress = (cur_y_float - p[0].y) / (p[2].y - p[0].y);
			cur_x = (int)roundf((p[2].x - p[0].x) * total_y_progress + p[0].x);
			float	y_progress =  (cur_y_float - p[0].y) / (p[1].y - p[0].y);
			int	x_max = (int)roundf((p[1].x - p[0].x) * y_progress + p[0].x);
			float cur_z;
			float start_z = y_progress * (projected->unprojected_z[1] - projected->unprojected_z[0]) + projected->unprojected_z[0];
			float end_z = total_y_progress * (projected->unprojected_z[2] - projected->unprojected_z[0]) + projected->unprojected_z[0];
			int	len_x = x_max - cur_x;
			float	start_x = cur_x;
			float	z_dist = end_z - start_z;
			int row_index = WIDTH * y_index;
			if ((start_z < Z_NEAR && end_z < Z_NEAR) || (start_z > Z_FAR && end_z > Z_FAR))
			{
				cur_y_float = cur_y_float + 1.0f;
				y_index =  (int)roundf(cur_y_float);
				continue ;
			}
			if (cur_x < x_max)
			{
				if (cur_x < 0)
					cur_x = 0;
				while (cur_x <= x_max && cur_x < WIDTH)
				{
					float x_progress = fabs(cur_x - start_x) / len_x;
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
					float x_progress = (cur_x - start_x) / len_x;
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
			cur_y_float = cur_y_float + 1.0f;
			y_index =  (int)roundf(cur_y_float);
		}
	}
	float	m2 = slope_2d_x_per_y(p[0], p[2]);
	float	m3 = slope_2d_x_per_y(p[1], p[2]);
	int x_max;
	cur_y_float = p[1].y;
	if (cur_y_float < 0.0f)
	{
		cur_y_float = 0.0f;
	}//todo: fix condtion for when objects leave the screen to the left and right
	//if ((p[2].x >= 0 || p[1].x >= 0) && (p[2].x < WIDTH || p[1].x < WIDTH) && (p[2].y >= 0 || p[1].y >= 0) && (p[2].y < HEIGHT || p[1].y < HEIGHT) && (p[2].z > Z_NEAR || p[1].z > Z_NEAR) && (projected->unprojected_z[2] < Z_FAR || projected->unprojected_z[1] < Z_FAR))
	{
		int y_index = (int)roundf(cur_y_float);
		while (cur_y_float <= p[2].y && y_index < HEIGHT)
		{
			float	y_progress =  (cur_y_float - p[1].y) / (p[2].y - p[1].y);
			total_y_progress = (cur_y_float - p[0].y) / (p[2].y - p[0].y);
			cur_x = (int)roundf((m3 * (cur_y_float - p[1].y) + p[1].x));
			x_max =  (int)roundf(((m2 * (cur_y_float - p[2].y) + p[2].x)));
			float cur_z;
			float start_z = y_progress * (projected->unprojected_z[2] - projected->unprojected_z[1]) + projected->unprojected_z[1];
			float end_z = y_progress * (projected->unprojected_z[2] - projected->unprojected_z[0]) + projected->unprojected_z[0];
			int	len_x = x_max - cur_x;
			float	start_x = cur_x;
			float	z_dist = end_z - start_z;
			int row_index = WIDTH * y_index;
			if ((start_z < Z_NEAR && end_z < Z_NEAR) || (start_z > Z_FAR && end_z > Z_FAR))
			{
				cur_y_float += 1.0f;
				y_index =  (int)roundf(cur_y_float);
				continue ;
			}
			if (cur_x < x_max)
			{
				if (cur_x < 0)
					cur_x = 0;
				
				while(cur_x <= x_max && cur_x < WIDTH)
				{
					float x_progress = (cur_x - start_x) / len_x;
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
					float x_progress = (cur_x - start_x) / len_x;
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
			cur_y_float += 1.0f;
			y_index =  (int)roundf(cur_y_float);
		}
	}
}
