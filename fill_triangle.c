/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 01:38:58 by frapp             #+#    #+#             */
/*   Updated: 2024/05/26 03:09:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

// int8_t	edge_fn(int a[2], int b[2], int p[2])
// {
// 	return (((b[X] - a[X]) * (p[Y] - a[Y] * (b[Y] - a[Y]) * (p[X] - a[X]))) > 0);
// }

void	sort_vertexes_for_y(t_triangle *tri)
{
	t_vec3	tmp;
	float	tmp_z;
	t_vec3	*p;

	p = tri->p;
	if (p[0].y > p[1].y)
	{
		tmp = p[0];
		tmp_z = tri->unprojected_z[0];
		p[0] = p[1];
		tri->unprojected_z[0] = tri->unprojected_z[1];
		p[1] = tmp;
		tri->unprojected_z[1] = tmp_z;
	}
	if (p[1].y > p[2].y)
	{
		tmp = p[1];
		tmp_z = tri->unprojected_z[1];
		p[1] = p[2];
		tri->unprojected_z[1] = tri->unprojected_z[2];
		p[2] = tmp;
		tri->unprojected_z[2] = tmp_z;
		if (p[0].y > p[1].y)
		{
			tmp = p[0];
			tmp_z = tri->unprojected_z[0];
			p[0] = p[1];
			tri->unprojected_z[0] = tri->unprojected_z[1];
			p[1] = tmp;
			tri->unprojected_z[1] = tmp_z;
		}
	}
}

float	slope_2d_x_per_y(t_vec3 p1, t_vec3 p2)
{
	if (p1.y == p2.y)
		return (0.0f);
	return ((p2.x - p1.x) / (p2.y - p1.y));
}

void	abs_uv(t_vec3	*p)
{
	p[0].u = fabs(p[0].u);
	p[0].v = fabs(p[0].v);
	p[1].u = fabs(p[1].u);
	p[1].v = fabs(p[1].v);
	p[2].u = fabs(p[2].u);
	p[2].v = fabs(p[2].v);
}

void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_scalars)
{
	t_vec3	*p = projected->p;
	float		*depth;
	uint32_t	*pixels = (uint32_t *)img->pixels;

	t_mtl	*mtl = projected->p->mtl;
	if (!mtl)
	{
		fprintf(stderr, "no mtl!\n");
		exit(1);
	}
	if (!mtl->texture)
	{
		fprintf(stderr, "no texture\n");
		exit(1);
	}
	abs_uv(p);//needed for now to remove negative values
	mlx_texture_t	*texture = mtl->texture;
	int texture_start_x = (texture->width - 1) * p->u;
	int texture_start_y = (texture->height - 1) * (1 - p->v);
	//fprintf(stderr, "y start: %d\n", )
	//uint32_t	color = ((uint32_t *)texture->pixels)[texture_start_x + texture_start_y * texture->width];
	
	int	center_x = (int)((texture->width - 1) *((p[0].u + p[1].u + p[2].u) / 3.0f));
	int	center_y = (int)((texture->height - 1) * ((3.0f - p[0].v - p[1].v - p[2].v) / (3.0f)));
	//fprintf(stderr, "width: %d height: %d\n", texture->width, texture->height);
	//fprintf(stderr, "center_x: %d center_y: %d\n", center_x, center_y);
	//fprintf(stderr, "v1: %f v2: %f v3: %f\n",  p[0].v, p[1].v, p[2].v);
	t_color_split	color = ((t_color_split *)texture->pixels)[center_x + center_y * texture->width];
	color.argb[R] *= color_scalars.v[R];
	color.argb[G] *= color_scalars.v[G];
	color.argb[B] *= color_scalars.v[B];

	depth = mesh->main->depth;
	sort_vertexes_for_y(projected);
	if (!(p[0].y <= p[1].y && p[1].y <= p[2].y))
	{
		fprintf(stderr, "error sort_vertexes_for_y 1: %f 2: %f 3: %f\n", p[0].y, p[1].y, p[2].y);
		exit(1);
	}
	// for (int i = 0; i < 3; i++)
	// {
	// 	if (p[i].x < 0 || p[i].x >= WIDTH)// || p[i].y < 0 ||  p[i].y >= HEIGHT)
	// 	{
	// 		print_vec3(p[0], 0);
	// 		print_vec3(p[1], 0);
	// 		print_vec3(p[2], 0);
	// 		exit(1);
	// 	}
	// }
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

			// if ((start_z < Z_NEAR && end_z < Z_NEAR) || (start_z > Z_FAR && end_z > Z_FAR))
			// {
			// 	cur_y_float = cur_y_float + 1.0f;
			// 	y_index =  (int)roundf(cur_y_float);
			// 	continue ;
			// }
			int	color_y_index = (y_progress * (1 - p[1].v - (1 - p[0].v)) + p[0].v) * (texture->height - 1);
			if (cur_x < x_max)
			{
				// if (cur_x < -100)
				// {
				// 	printf("cur_x < 0: cur_x: %d\n", cur_x);
				// 	exit(1);
				// }
				if (cur_x < 0)
					cur_x = 0;
				while (cur_x <= x_max && cur_x < WIDTH)
				{
					float x_progress = fabs(cur_x - start_x) / len_x;
					cur_z = x_progress * z_dist + start_z;
					int fin_index = cur_x + row_index;
					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
					{
						// float	cur_x_total_progress;
						// if (cur_x > p[0].x && cur_x < p[1].x)
						// {
						// 	if (!zero_f(p[1].x - p[0].x))
						// 	{
						// 		cur_x_total_progress = (cur_x - p[0].x) / (p[1].x - p[0].x); //p0 to p1
						// 	}
						// 	else
						// 	{
						// 		cur_x_total_progress = 1.0f;
						// 	}
						// 	// if (cur_x_total_progress > 0)
						// 	// 	cur_x_total_progress = 1.0f;
						// 	int	cur_x_color_index = (texture->width - 1) * ((cur_x_total_progress * (p[1].u - p[0].u)) + p[0].u);
						// 	fprintf(stderr, "total_x_progress: %f color x index: %d color y index: %d\n", cur_x_total_progress, cur_x_color_index, color_y_index);
						// 	color =  ((uint32_t *)texture->pixels)[cur_x_color_index + color_y_index];
						// }
						depth[fin_index] = cur_z;
						pixels[fin_index] = color.col;
					}
					cur_x++;
				}
			}
			else if (cur_x > x_max)
			{
				// if (cur_x >= WIDTH)
				// {
				// 	printf("cur_x >= WIDTH: cur_x: %d\n", cur_x);
				// 	exit(1);
				// }
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
						pixels[fin_index] = color.col;
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
				{
					printf("cur_x: %d\n", cur_x);
					cur_x = 0;
				}
				while(cur_x <= x_max && cur_x < WIDTH)
				{
					float x_progress = (cur_x - start_x) / len_x;
					cur_z = x_progress * z_dist + start_z;
					int fin_index = cur_x + row_index;
					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
					{
						depth[fin_index] = cur_z;
						pixels[fin_index] = color.col;
					}
					cur_x++;
				}
			}
			else if (cur_x > x_max)
			{
				if (cur_x >= WIDTH)
				{
					printf("cur_x: %d\n", cur_x);
					cur_x = WIDTH - 1;
				}
				while (cur_x >= x_max && cur_x >= 0)
				{
					float x_progress = (cur_x - start_x) / len_x;
					cur_z = x_progress * z_dist + start_z;
					int fin_index = cur_x + row_index;
					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
					{
						depth[fin_index] = cur_z;
						pixels[fin_index] = color.col;
					}
					cur_x--;
				}
			}
			cur_y_float += 1.0f;
			y_index =  (int)roundf(cur_y_float);
		}
	}
}
