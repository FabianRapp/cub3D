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

uint32_t	load_pixel_from_mlx_texture(mlx_texture_t *texture, double u, double v)
{
	const	uint32_t	width = texture->width;
	const	uint32_t	height = texture->height;
	uint32_t	*buffer = (uint32_t *)texture->pixels;

	assume(texture->width && texture->height);
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
	int x_index = round((texture->width - 1) * (u));
	int y_index = round((texture->height - 1) * (v));
	//y_index = ((height - 1) * (1 - y));
	//x_index = ((width - 1) * (1 - x));
	assume(x_index >= 0 && y_index >= 0);
	if (x_index < 0)
		x_index = 0;
	if (y_index < 0)
		y_index = 0;
	if (!(x_index < texture->width))
			printf("x_index: %d width: %u u: %lf\n", x_index, texture->width, u);
	if (!(y_index < texture->height))
			printf("y_index: %d height: %u v: %lf\n", y_index, texture->height, v);
	const	uint32_t	i = (width * y_index + x_index);
	return (buffer[i]);
}

void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_sclars)
{
	t_vec3	*p = projected->p;
	double		*depth;
	uint32_t	*pixels = (uint32_t *)img->pixels;
	t_mtl	*mtl = projected->p->mtl;
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

	double	unprojected_z_dist10 = p[1].z- p[0].z;
	double	unprojected_z_dist20 = p[2].z- p[0].z;

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
			first_col_z = total_y_progress * unprojected_z_dist20 + p[0].z;
			last_col_z = section_y_progress * unprojected_z_dist10 + p[0].z;
			first_col_u = total_y_progress * u_dist20 + p[0].u;
			last_col_u = total_y_progress * u_dist10 + p[0].u;
			first_col_v = total_y_progress * v_dist20 + p[0].v;
			last_col_v = total_y_progress * v_dist10 + p[0].v;
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
					pixels[fin_index] = load_pixel_from_mlx_texture(mtl->texture, cur_u, cur_v);
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
	double	unprojected_z_dist21 = p[2].z - p[1].z;
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
		double first_col_z = total_y_progress * unprojected_z_dist20 + p[0].z;
		double last_col_z = section_y_progress * unprojected_z_dist21 + p[1].z;
		double	cur_u;
		double	first_col_u;
		double	last_col_u;
		double	cur_v;
		double	first_col_v;
		double	last_col_v;
		first_col_z = total_y_progress * unprojected_z_dist20 + p[0].z;
		last_col_z = section_y_progress * unprojected_z_dist10 + p[0].z;
		first_col_u = total_y_progress * u_dist20 + p[0].u;
		last_col_u = total_y_progress * u_dist10 + p[0].u;
		first_col_v = total_y_progress * v_dist20 + p[0].v;
		last_col_v = total_y_progress * v_dist10 + p[0].v;

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
				cur_u = first_col_u + t * (last_col_u - first_col_u);
				cur_v = first_col_v + t * (last_col_v - first_col_v);
				//cur_u = p[1].u;
				//cur_v = p[1].v;
				pixels[fin_index] = load_pixel_from_mlx_texture(mtl->texture, cur_u, cur_v);
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

//void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_scalars)
//{
//	t_vec3	*p = projected->p;
//	double		*depth;
//	uint32_t	*pixels = (uint32_t *)img->pixels;
//
//	t_mtl	*mtl = projected->p->mtl;
//	if (!mtl)
//	{
//		fprintf(stderr, "no mtl!\n");
//		exit(1);
//	}
//	if (!mtl->texture)
//	{
//		fprintf(stderr, "no texture\n");
//		exit(1);
//	}
//	abs_uv(p);//needed for now to remove negative values
//	mlx_texture_t	*texture = mtl->texture;
//	int texture_start_x = (texture->width - 1) * p->u;
//	int texture_start_y = (texture->height - 1) * (1 - p->v);
//	//fprintf(stderr, "y start: %d\n", )
//	//uint32_t	color = ((uint32_t *)texture->pixels)[texture_start_x + texture_start_y * texture->width];
//	
//	int	center_x = (int)((texture->width - 1) *((p[0].u + p[1].u + p[2].u) / 3.0f));
//	int	center_y = (int)((texture->height - 1) * ((3.0f - p[0].v - p[1].v - p[2].v) / (3.0f)));
//	//fprintf(stderr, "width: %d height: %d\n", texture->width, texture->height);
//	//fprintf(stderr, "center_x: %d center_y: %d\n", center_x, center_y);
//	//fprintf(stderr, "v1: %f v2: %f v3: %f\n",  p[0].v, p[1].v, p[2].v);
//	t_color_split	color = ((t_color_split *)texture->pixels)[center_x + center_y * texture->width];
//	color.argb[R] *= color_scalars.v[R];
//	color.argb[G] *= color_scalars.v[G];
//	color.argb[B] *= color_scalars.v[B];
//
//	depth = mesh->main->depth;
//	sort_vertexes_for_y(projected);
//	if (!(p[0].y <= p[1].y && p[1].y <= p[2].y))
//	{
//		fprintf(stderr, "error sort_vertexes_for_y 1: %f 2: %f 3: %f\n", p[0].y, p[1].y, p[2].y);
//		exit(1);
//	}
//	// for (int i = 0; i < 3; i++)
//	// {
//	// 	if (p[i].x < 0 || p[i].x >= WIDTH)// || p[i].y < 0 ||  p[i].y >= HEIGHT)
//	// 	{
//	// 		print_vec3(p[0], 0);
//	// 		print_vec3(p[1], 0);
//	// 		print_vec3(p[2], 0);
//	// 		exit(1);
//	// 	}
//	// }
//	//double	m1 = slope_2d_x_per_y(p[0], p[1]);
//	double	y_dist1 = p[1].y - p[0].y;
//	double	cur_y_double = p[0].y;
//	double	total_y_progress;
//	int		cur_x;
//	static int i = 0;
//	if (cur_y_double < 0.0f)
//	{
//		cur_y_double = 0.0f;
//	}//todo: fix condtion for when objects leave the screen to the left and right
//	//if ((p[0].x >= 0 || p[1].x >= 0) && (p[0].x < WIDTH || p[1].x < WIDTH) && (p[0].y >= 0 || p[1].y >= 0) && (p[0].y < HEIGHT || p[1].y < HEIGHT) && (p[0].z > Z_NEAR || p[1].z > Z_NEAR) && (p[0].unprojected_z < Z_FAR || p[1].unprojected_z < Z_FAR))
//	{
//		int y_index =  (int)roundf(cur_y_double);
//		while (cur_y_double <= p[1].y && y_index < HEIGHT)
//		{
//			total_y_progress = (cur_y_double - p[0].y) / (p[2].y - p[0].y);
//			cur_x = (int)roundf((p[2].x - p[0].x) * total_y_progress + p[0].x);
//			double	y_progress =  (cur_y_double - p[0].y) / (p[1].y - p[0].y);
//			int	x_max = (int)roundf((p[1].x - p[0].x) * y_progress + p[0].x);
//			double cur_z;
//			double start_z = y_progress * (p[1].unprojected_z - p[0].unprojected_z) + p[0].unprojected_z;
//			double end_z = total_y_progress * (p[2].unprojected_z - p[0].unprojected_z) + p[0].unprojected_z;
//			int	len_x = x_max - cur_x;
//			double	start_x = cur_x;
//			double	z_dist = end_z - start_z;
//			int row_index = WIDTH * y_index;
//
//			// if ((start_z < Z_NEAR && end_z < Z_NEAR) || (start_z > Z_FAR && end_z > Z_FAR))
//			// {
//			// 	cur_y_double = cur_y_double + 1.0f;
//			// 	y_index =  (int)roundf(cur_y_double);
//			// 	continue ;
//			// }
//			int	color_y_index = (y_progress * (1 - p[1].v - (1 - p[0].v)) + p[0].v) * (texture->height - 1);
//			if (cur_x < x_max)
//			{
//				// if (cur_x < -100)
//				// {
//				// 	printf("cur_x < 0: cur_x: %d\n", cur_x);
//				// 	exit(1);
//				// }
//				if (cur_x < 0)
//					cur_x = 0;
//				while (cur_x <= x_max && cur_x < WIDTH)
//				{
//					double x_progress = fabs(cur_x - start_x) / len_x;
//					cur_z = x_progress * z_dist + start_z;
//					int fin_index = cur_x + row_index;
//					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
//					{
//						// double	cur_x_total_progress;
//						// if (cur_x > p[0].x && cur_x < p[1].x)
//						// {
//						// 	if (!zero_f(p[1].x - p[0].x))
//						// 	{
//						// 		cur_x_total_progress = (cur_x - p[0].x) / (p[1].x - p[0].x); //p0 to p1
//						// 	}
//						// 	else
//						// 	{
//						// 		cur_x_total_progress = 1.0f;
//						// 	}
//						// 	// if (cur_x_total_progress > 0)
//						// 	// 	cur_x_total_progress = 1.0f;
//						// 	int	cur_x_color_index = (texture->width - 1) * ((cur_x_total_progress * (p[1].u - p[0].u)) + p[0].u);
//						// 	fprintf(stderr, "total_x_progress: %f color x index: %d color y index: %d\n", cur_x_total_progress, cur_x_color_index, color_y_index);
//						// 	color =  ((uint32_t *)texture->pixels)[cur_x_color_index + color_y_index];
//						// }
//						depth[fin_index] = cur_z;
//						pixels[fin_index] = color.col;
//					}
//					cur_x++;
//				}
//			}
//			else if (cur_x > x_max)
//			{
//				// if (cur_x >= WIDTH)
//				// {
//				// 	printf("cur_x >= WIDTH: cur_x: %d\n", cur_x);
//				// 	exit(1);
//				// }
//				if (cur_x >= WIDTH)
//					cur_x = WIDTH - 1;
//				while (cur_x >= x_max && cur_x >= 0)
//				{
//					double x_progress = (cur_x - start_x) / len_x;
//					cur_z = x_progress * z_dist + start_z;
//					int fin_index = cur_x + row_index;
//					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
//					{
//						depth[fin_index] = cur_z;
//						pixels[fin_index] = color.col;
//					}
//					cur_x--;
//				}
//			}
//			cur_y_double = cur_y_double + 1.0f;
//			y_index =  (int)roundf(cur_y_double);
//		}
//	}
//	double	m2 = slope_2d_x_per_y(p[0], p[2]);
//	double	m3 = slope_2d_x_per_y(p[1], p[2]);
//	int x_max;
//	cur_y_double = p[1].y;
//	if (cur_y_double < 0.0f)
//	{
//		cur_y_double = 0.0f;
//	}//todo: fix condtion for when objects leave the screen to the left and right
//	//if ((p[2].x >= 0 || p[1].x >= 0) && (p[2].x < WIDTH || p[1].x < WIDTH) && (p[2].y >= 0 || p[1].y >= 0) && (p[2].y < HEIGHT || p[1].y < HEIGHT) && (p[2].z > Z_NEAR || p[1].z > Z_NEAR) && (p[2].unprojected_z < Z_FAR || p[1].unprojected_z < Z_FAR))
//	{
//		int y_index = (int)roundf(cur_y_double);
//		while (cur_y_double <= p[2].y && y_index < HEIGHT)
//		{
//			double	y_progress =  (cur_y_double - p[1].y) / (p[2].y - p[1].y);
//			total_y_progress = (cur_y_double - p[0].y) / (p[2].y - p[0].y);
//			cur_x = (int)roundf((m3 * (cur_y_double - p[1].y) + p[1].x));
//			x_max =  (int)roundf(((m2 * (cur_y_double - p[2].y) + p[2].x)));
//			double cur_z;
//			double start_z = y_progress * (p[2].unprojected_z - p[1].unprojected_z) + p[1].unprojected_z;
//			double end_z = y_progress * (p[2].unprojected_z - p[0].unprojected_z) + p[0].unprojected_z;
//			int	len_x = x_max - cur_x;
//			double	start_x = cur_x;
//			double	z_dist = end_z - start_z;
//			int row_index = WIDTH * y_index;
//			if ((start_z < Z_NEAR && end_z < Z_NEAR) || (start_z > Z_FAR && end_z > Z_FAR))
//			{
//				cur_y_double += 1.0f;
//				y_index =  (int)roundf(cur_y_double);
//				continue ;
//			}
//			if (cur_x < x_max)
//			{
//				if (cur_x < 0)
//				{
//					printf("cur_x: %d\n", cur_x);
//					cur_x = 0;
//				}
//				while(cur_x <= x_max && cur_x < WIDTH)
//				{
//					double x_progress = (cur_x - start_x) / len_x;
//					cur_z = x_progress * z_dist + start_z;
//					int fin_index = cur_x + row_index;
//					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
//					{
//						depth[fin_index] = cur_z;
//						pixels[fin_index] = color.col;
//					}
//					cur_x++;
//				}
//			}
//			else if (cur_x > x_max)
//			{
//				if (cur_x >= WIDTH)
//				{
//					printf("cur_x: %d\n", cur_x);
//					cur_x = WIDTH - 1;
//				}
//				while (cur_x >= x_max && cur_x >= 0)
//				{
//					double x_progress = (cur_x - start_x) / len_x;
//					cur_z = x_progress * z_dist + start_z;
//					int fin_index = cur_x + row_index;
//					if (cur_z > Z_NEAR && cur_z < depth[fin_index])
//					{
//						depth[fin_index] = cur_z;
//						pixels[fin_index] = color.col;
//					}
//					cur_x--;
//				}
//			}
//			cur_y_double += 1.0f;
//			y_index =  (int)roundf(cur_y_double);
//		}
//	}
//}
