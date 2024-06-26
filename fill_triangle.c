/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 01:38:58 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
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
	t_vec3	*p;

	p = tri->p;
	if (p[0].y > p[1].y)
	{
		tmp = p[0];
		p[0] = p[1];
		p[1] = tmp;
	}
	if (p[1].y > p[2].y)
	{
		tmp = p[1];
		p[1] = p[2];
		p[2] = tmp;
		if (p[0].y > p[1].y)
		{
			tmp = p[0];
			p[0] = p[1];
			p[1] = tmp;
		}
	}
}

double	slope_2d_x_per_y(t_vec3 p1, t_vec3 p2)
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

//void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_scalars)

static inline uint32_t	load_pixel_from_mlx_texture(mlx_texture_t texture, double u, double v)
{
	const	uint32_t	width = texture.width;
	const	uint32_t	height = texture.height;
	uint32_t	*buffer = (uint32_t *)texture.pixels;

	assume(texture.width && texture.height);
	if (u < 0.0)
	{
		//printf("%lf\n", u);
		u = 0.0;
	}
	else if (u > 1.0)
	{
		//printf("%lf\n", x);
		u = 1.0;
	}
	if (v < 0.0)
	{
		//printf("%lf\n", y);
		v = 0.0;
	}
	else if (v >= 1.0)
	{
		//printf("%lf\n", y);
		v = 1.0;
	}
	int x_index = round((texture.width - 1) * (u));
	int y_index = round((texture.height - 1) * (v));
	assume(x_index >= 0 && y_index >= 0);
	return (buffer[width * y_index + x_index]);
}

void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_sclars)
{
	t_vec3	*p = projected->p;
	double		*depth;
	uint32_t	*pixels = (uint32_t *)img->pixels;
	t_mtl	*mtl = projected->p->mtl;
	const mlx_texture_t	texture = *(mtl->texture);
	int a = 0;
	assume(mtl);
	uint32_t	color = RED;
	//static int color_index = 0;
	//color = colors[color_index++];
	//if (color_index >= sizeof colors / sizeof(uint32_t))
	//	color_index = 0;
	depth = mesh->main->depth;
	sort_vertexes_for_y(projected);
	assume(p[0].y <= p[1].y && p[1].y <= p[2].y);
	for (int i = 0; i < 3; i++)
	{
		p[i].v = 1.0 - p[i].v;
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

	double	z_dist10 = p[1].z- p[0].z;
	double	z_dist20 = p[2].z- p[0].z;

	double	u_dist10 = p[1].u - p[0].u;
	double	u_dist20 = p[2].u - p[0].u;

	double	v_dist10 = p[1].v - p[0].v;
	double	v_dist20 = p[2].v - p[0].v;

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
			double	cur_u;
			double	first_col_u;
			double	last_col_u;
			double	cur_v;
			double	first_col_v;
			double	last_col_v;
			first_col_z = total_y_progress * z_dist20 + p[0].z;
			last_col_z = section_y_progress * z_dist10 + p[0].z;
			first_col_u = total_y_progress * u_dist20 + p[0].u;
			last_col_u = section_y_progress * u_dist10 + p[0].u;
			first_col_v = total_y_progress * v_dist20 + p[0].v;
			last_col_v = section_y_progress * v_dist10 + p[0].v;
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
					cur_u = first_col_u + t * (last_col_u - first_col_u);
					cur_v = first_col_v + t * (last_col_v - first_col_v);
					//cur_u = p[0].u;
					//cur_v = p[0].v;
					pixels[fin_index] = load_pixel_from_mlx_texture(texture, cur_u, cur_v);
					//pixels[fin_index] = RED;
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
	double	u_dist21 = p[2].u - p[1].u;
	double	v_dist21 = p[2].v - p[1].v;
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
		double	cur_u;
		double	first_col_u;
		double	last_col_u;
		double	cur_v;
		double	first_col_v;
		double	last_col_v;
		first_col_z = total_y_progress * z_dist20 + p[0].z;
		last_col_z = section_y_progress * z_dist21 + p[1].z;
		first_col_u = total_y_progress * u_dist20 + p[0].u;
		last_col_u = section_y_progress * u_dist21 + p[1].u;
		first_col_v = total_y_progress * v_dist20 + p[0].v;
		last_col_v = section_y_progress * v_dist21 + p[1].v;

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
double t = (cur_col - first_col) / (double)len_x;
cur_z = first_col_z + t * (last_col_z - first_col_z);
			int fin_index = cur_col + row_start_offset;
			if (cur_z < depth[fin_index])//should be prefetched
			{
				depth[fin_index] = cur_z;
				cur_u = first_col_u + t * (last_col_u - first_col_u);
				cur_v = first_col_v + t * (last_col_v - first_col_v);
				pixels[fin_index] = load_pixel_from_mlx_texture(texture, cur_u, cur_v);
				//pixels[fin_index] = RED;
			}
			if (first_col == last_col)
				break ;
				//return ;
			cur_col += direct_x;
		}
		cur_y_lf += 1.0f;
		row_index =  (int)round(cur_y_lf);
	}
}

