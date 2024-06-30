/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 01:39:06 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

void	draw_line(mlx_image_t *image, int x1, int x2, int y1, int y2, int color)
{
	int	pos[2];
	int	dist_x;
	int	dist_y;
	int	direct_x;
	int	direct_y;
	int	last_error;
	int	cur_error;

	if ((x1 < 0 && x2 < 0) || (x1 >= WIDTH && x2 >= WIDTH)
		|| (y1 < 0 && y2 < 0) || y1 >= HEIGHT && y2 >= HEIGHT)
	{
		return ;
	}
	if (x1 < 0)
		x1 = 0;
	if (x1 >= WIDTH)
		x1 = WIDTH - 1;
	if (x2 < 0)
		x2 = 0;
	if (x2 >= WIDTH)
		x2 = WIDTH - 1;
	if (y1 < 0)
		y1 = 0;
	if (y1 >= WIDTH)
		y1 = WIDTH - 1;
	if (y2 < 0)
		y2 = 0;
	if (y2 >= WIDTH)
		y2 = WIDTH - 1;
	if (x1 == x2 && y1 == y2)
	{
		ft_put_pixel(image->pixels, x1, y1, color);
		return ;
	}
	pos[X] = x1;
	pos[Y] = y1;
	direct_x = (pos[X] > x2) * -1;
	direct_y = (pos[Y] > y2) * -1;
	if (!direct_x)
		direct_x++;
	if (!direct_y)
		direct_y++;
	dist_x = abs(x2 - pos[X]);
	dist_y = abs(y2 - pos[Y]);
	if (pos[X] == x2 || pos[Y] == y2)
	{
		if (pos[X] == x2)
		{
			while (pos[Y] != y2)
			{
				if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
					ft_put_pixel(image->pixels, pos[X], pos[Y], color);
					//mlx_put_pixel(image, pos[X], pos[Y], color);
				pos[Y] += direct_y;
			}
		}
		if (pos[Y] == y2)
		{
			while (pos[X] != x2)
			{
				if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
					ft_put_pixel(image->pixels, pos[X], pos[Y], color);
				pos[X] += direct_x;
			}
		}
		if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
			ft_put_pixel(image->pixels, pos[X], pos[Y], color);
			//mlx_put_pixel(image, pos[X], pos[Y], color);
		//else
			//fprintf(stderr, "error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
		return ;
	}

	last_error = dist_x - dist_y;
	while (pos[X] != x2 || pos[Y] != y2)
	{
		//fprintf(stderr, "x: %d y: %d\n", pos[X], pos[Y]);
		if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
			ft_put_pixel(image->pixels, pos[X], pos[Y], color);
			//mlx_put_pixel(image, pos[X], pos[Y], color);
	//	else
			//fprintf(stderr, "error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
		cur_error = 2 * last_error;
		if (cur_error >= -dist_y)
		{
			last_error -= dist_y;
			pos[X] += direct_x;
		}
		if (cur_error < dist_x)
		{
			last_error += dist_x;
			pos[Y] += direct_y;
		}
	}
	if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
		ft_put_pixel(image->pixels, pos[X], pos[Y], color);
		//mlx_put_pixel(image, pos[X], pos[Y], color);
	else {
		//fprintf(stderr, "error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
	}
}

void	draw_triangle(mlx_image_t *img, t_triangle *projected, uint32_t color)
{
	draw_line(img, (int)roundf(projected->p->x), (int)roundf(projected->p[1].x), (int)roundf(projected->p->y), (int)roundf(projected->p[1].y), color);
	draw_line(img, (int)roundf(projected->p[1].x), (int)roundf(projected->p[2].x), (int)roundf(projected->p[1].y), (int)roundf(projected->p[2].y), color);
	draw_line(img, (int)roundf(projected->p[2].x), (int)roundf(projected->p->x), (int)roundf(projected->p[2].y), (int)roundf(projected->p->y), color);
}

double	norm_double(double val, double min, double max)
{
	return (val / (max - min));
}

void	print_color(t_color_split color)
{
	t_color_split	col;

	//col.col = color;
	col = color;
	fprintf(stderr, "total color: %x\n", col.col);
	fprintf(stderr, "a: %x r: %x g: %x b: %x\n", col.argb[A], col.argb[R], col.argb[G], col.argb[B]);
}

void	init_light(t_light *light, t_vec3 direct, uint32_t color, double base_stren)
{
	light->direct = direct;
	unit_vec3(&light->direct);
	light->color.col = color;
	//light->strength.v[A] = light->color.argb[A] / (0xFF * base_stren);
	light->strength.v[R] = light->color.argb[R] / (0xFF * base_stren);
	light->strength.v[G] = light->color.argb[G] / (0xFF * base_stren);
	light->strength.v[B] = light->color.argb[B] / (0xFF * base_stren);
}

void	scale_to_screen(t_triangle *projected)
{
	projected->p[0].x += 1.0f;
	projected->p[1].x += 1.0f;
	projected->p[0].y += 1.0f;
	projected->p[1].y += 1.0f;
	projected->p[2].x += 1.0f;
	projected->p[2].y += 1.0f;

	projected->p[0].x *= 0.5f * (double)WIDTH;
	projected->p[0].y *= 0.5f * (double)HEIGHT;
	projected->p[1].x *= 0.5f * (double)WIDTH;
	projected->p[1].y *= 0.5f * (double)HEIGHT;
	projected->p[2].x *= 0.5f * (double)WIDTH;
	projected->p[2].y *= 0.5f * (double)HEIGHT;
}

// can be optimized alot
// not all the steps have to be done all the time
// (for example for most object the scaling should be fine during the init
// and other objects might not move at all)
void	fill_model_matrix(t_model_space_data *data)
{
	double	translation_mat[4][4];
	double	scale_mat[4][4];
	double	rotx_mat[4][4];
	double	roty_mat[4][4];
	double	rotz_mat[4][4];

	//data->x_scale = 20;
	//data->y_scale = 20;
	//data->z_scale = 20;
	translation_matrix(translation_mat, data->x_translation, data->y_translation, data->z_translation);
	scale_matrix(scale_mat, data->x_scale, data->y_scale, data->z_scale);
	rot_matx_4x4(rotx_mat, data->x_rotation);
	rot_maty_4x4(roty_mat, data->y_rotation);
	rot_matz_4x4(rotz_mat, data->z_rotation);

	double	tmp1[4][4];
	mat4x4_mult_mat4x4(rotx_mat, scale_mat, tmp1);
	
	double	tmp2[4][4];
	mat4x4_mult_mat4x4(tmp1, roty_mat, tmp2);

	double	tmp3[4][4];
	mat4x4_mult_mat4x4(tmp2, rotz_mat, tmp3);

	double	tmp4[4][4];
	mat4x4_mult_mat4x4(tmp3, translation_mat, tmp4);

	ft_memcpy(data->model_matrix, tmp4, sizeof(double[4][4]));
}

t_light	init_day_light(double d_time)
{
	t_light			day_light;
	static t_vec3	direct =  {.x = 1.0f, .y = -1.0f, .z = -1.0f, .w = 0};
	static double light_direct = -1;

	if (direct.x < -1)
		light_direct = 1;
	if (direct.x > 1)
		light_direct = -1;
	direct.x += d_time * light_direct / 50;
	double day_progress = (direct.x + 1) / 2;
	static double timer = 0;
	timer += d_time;
	if (timer > 1)
	{
		//fprintf(stderr, "day time: %f\n", 24 * day_progress);
		timer = 0;
	}
	double	light_intens = 1 - fabs(0.5 - day_progress);
	t_color_split light_col;
	light_col.col = WHITE;
	light_col.argb[R] *= 1 - 0.2 * fabs(0.5 - day_progress);
	light_col.argb[G] *= 1 - 0.4 * fabs(0.5 - day_progress);
	light_col.argb[B] *= 1 - 0.8 * fabs(0.5 - day_progress);
	init_light(&day_light, direct, light_col.col, 1 - fabs(0.5 - day_progress));
	return (day_light);
}

//copies specifc tri data
void	cpy_triangle_data_rasterize(t_triangle *src, t_triangle *dst)
{
	int	i;

	ft_memcpy(&dst->centroid, &src->centroid, sizeof(t_triangle) - (((uintptr_t)(&src->centroid)) - ((uintptr_t)src)));
	i = 0;
	while (i < 3)
	{
		ft_memcpy(&(dst->p + i)->mtl, &(src->p + i)->mtl, sizeof(t_vec3) - ((((uintptr_t)&(src->p + i)->mtl)) - ((uintptr_t)(src->p + i))));
		i++;
	}
}

// TODO: the clipping loops are inefficent and need refactor anyay
void	rasterize(t_triangle triangle, t_mesh *mesh, t_triangle *base_data, t_light_argb_stren color_scalars)
{
	t_triangle	clipped_z_front[2];
	t_triangle	clipped_z_back[2];
	
	int			clipped_count_front;
	int			clipped_count_back;
	t_triangle	projected;
	double	project_mat[4][4] = PROJECTION_MATRIX;
	projection_matrix(project_mat);

	clipped_count_front = clipping_z_near(&triangle, clipped_z_front);
	int	j;
	int	q;
	j = 0;
	while (j < clipped_count_front)
	{
		triangle = clipped_z_front[j];
		//todo: clipping_z_far not active for debugging
		//clipped_count_back = clipping_z_far(&triangle, clipped_z_back);
		clipped_z_back[0] = triangle;
		clipped_count_back = 1;
		q = 0;
		uint32_t col = clipped_z_front[j].col; //TODO remove this line after fixing/implenting clipping
		while (q < clipped_count_back)
		{
			triangle = clipped_z_back[q];
			// enter projeceted space
			projected = triangle;
			for (int i = 0; i < 3; i++)
			{
				//matrix_mult_vec3_4x4(triangle.p + i, project_mat, &projected.p[i])
				projected.p[i].z = triangle.p[i].z * (Z_FAR / (Z_FAR - Z_NEAR)) - ((Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR));
				projected.p[i].z /= triangle.p[i].z;
				projected.p[i].w = 1 * triangle.p[i].z;
				projected.p[i].x = -1.0 * triangle.p[i].x * ASPECT_RATIO * (1 / tan(FOV_RAD / 2));
				projected.p[i].x /= triangle.p[i].z;
				projected.p[i].y = -1.0 * triangle.p[i].y * (1 / tan(FOV_RAD / 2));
				projected.p[i].y /= triangle.p[i].z;
			}
			cpy_triangle_data_rasterize(base_data, &projected);
			for (int i = 0; i < 3; i++)
			{
				assume(!zero_f(projected.p[i].w));
				//div_vec3(projected.p + i, projected.p[i].w);
			}

			scale_to_screen(&projected); // todo: this could be after xy-
			// clipping to simplify clipping to values 0 to 1 (needs
			t_triangle				clipped[30];
			clipped[0] = projected;
			int clipped_count = call_clipping_xy(clipped);

			int l = 0;
			while (l < clipped_count)
			{
				projected = clipped[l];
				//cpy_triangle_data_rasterize(base_data, &projected);
				for (int i = 0; i < 3; i++)
				{
					if (!(projected.p[i].x >= 0.0f && projected.p[i].x < (double)WIDTH
						&& projected.p[i].y >= 0.0f && projected.p[i].y < HEIGHT))
					{
						print_vec3(projected.p[i], "out of bounds:");
						assume(0);
					}
				}
				if (!projected.p[0].mtl || !projected.p[0].mtl->texture.buffer.arr)
					fill_triangle_color(mesh->img, &projected, projected.col, mesh);
					//fill_triangle_color(mesh->img, &projected, base_data->col, mesh);
				else
					fill_triangle_texture(mesh->img, &projected, mesh, color_scalars);
				l++;
			}
			q++;
		}
		j++;
	}
}

void	mesh_physics_handler(t_mesh *mesh)
{
}

//Model to (World Space skipped), then to Camera and then Projection
void	draw_mesh(t_mesh *mesh)
{
	int			i;
	t_triangle	model;
	t_triangle	viewed;
	double		mat_model_world_space[4][4];

	mesh_physics_handler(mesh);
	fill_model_matrix(&mesh->model_space_data);

	t_vec3	vec_target = v3_add(mesh->world_data->camera, mesh->world_data->look_direct);
	double	camera[4][4];
	matrix_point_at(&mesh->world_data->camera, &vec_target, &mesh->world_data->up, camera);
	//print_vec3(mesh->main->up, "up");
	double	mat_view[4][4];
	matrix_look_at(camera, mat_view);
	

	t_light	day_light;

	day_light = init_day_light(*mesh->physics_data.delta_time);


	t_light			ambient_light2;
	const t_vec3	light_direct2 =  {1.0f, -1.0f, -1.0f};
	init_light(&ambient_light2, light_direct2, WHITE, 0.0f);

	// t_light			ambient_light3;
	// const t_vec3	light_direct3 =  {-1.0f, 1.0f, -1.0f};
	// init_light(&ambient_light3, light_direct3, WHITE);
	t_vec3	bounds_result;
	i = 0;
	while (i < mesh->count)
	{

		model = mesh->triangles[i];//not neededm, for debugging
		//Model space
		mesh->triangles[i].p[0].w = 1.0;
		mesh->triangles[i].p[1].w = 1.0;
		mesh->triangles[i].p[2].w = 1.0;
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 0, mesh->model_space_data.model_matrix, model.p + 0);
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 1, mesh->model_space_data.model_matrix, model.p + 1);
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 2, mesh->model_space_data.model_matrix, model.p + 2);

		// World Space (but not yet converted)
		// check if triangle is facing away from the view direction
		model.normal = cross_product(v3_sub(model.p[1], model.p[0]), v3_sub(model.p[2], model.p[0]));
		unit_vec3(&model.normal);
		//if (dot_prod_unit(model.normal, v3_sub(model.p[0], mesh->world_data->camera)) >= 0)
		if (dot_prod_unit(model.normal, v3_sub(model.p[0], mesh->world_data->camera)) < 0)
		{
			i++;
			continue ;
		}

		t_light_argb_stren	color_scalars = {0};

		double light_dp = dot_prod_unit(model.normal, day_light.direct);
		light_dp = fmaxf(light_dp, 0.0f);
		color_scalars.v[R] += day_light.strength.v[R] *  light_dp;
		color_scalars.v[G] += day_light.strength.v[G] *  light_dp;
		color_scalars.v[B] += day_light.strength.v[B] *  light_dp;

		// light_dp = dot_prod_unit(transformed.normal, ambient_light2.direct);
		// light_dp = fmaxf(light_dp, 0.0f);
		// color_scalars.v[R] += ambient_light2.strength.v[R] *  light_dp;
		// color_scalars.v[G] += ambient_light2.strength.v[G] *  light_dp;
		// color_scalars.v[B] += ambient_light2.strength.v[B] *  light_dp;

		// light_dp = dot_prod_unit(transformed.normal, ambient_light3.direct);
		// light_dp = fmaxf(light_dp, 0.0f);
		// color_scalars.v[R] += ambient_light3.strength.v[R] *  light_dp;
		// color_scalars.v[G] += ambient_light3.strength.v[G] *  light_dp;
		// color_scalars.v[B] += ambient_light3.strength.v[B] *  light_dp;

		color_scalars.v[R] = fminf(color_scalars.v[R], 1.0f);
		color_scalars.v[G] = fminf(color_scalars.v[G], 1.0f);
		color_scalars.v[B] = fminf(color_scalars.v[B], 1.0f);

		// color.argb[R] *= color_scalars.v[R];
		// color.argb[G] *= color_scalars.v[G];
		// color.argb[B] *= color_scalars.v[B];

		// enter view space
		viewed = model;
		for (int i = 0; i < 3; i++)
		{
			matrix_mult_vec3_4x4(model.p + i, mat_view, viewed.p + i);
		}
		rasterize(viewed, mesh, mesh->triangles + i, color_scalars);
		i++;
	}
}
