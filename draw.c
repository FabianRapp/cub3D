/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 01:39:06 by frapp             #+#    #+#             */
/*   Updated: 2024/05/09 03:07:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

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

float	norm_float(float val, float min, float max)
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

void	init_light(t_light *light, t_vec3 direct, uint32_t color, float base_stren)
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

	projected->p[0].x *= 0.5f * (float)WIDTH;
	projected->p[0].y *= 0.5f * (float)HEIGHT;
	projected->p[1].x *= 0.5f * (float)WIDTH;
	projected->p[1].y *= 0.5f * (float)HEIGHT;
	projected->p[2].x *= 0.5f * (float)WIDTH;
	projected->p[2].y *= 0.5f * (float)HEIGHT;
}

void	fill_mesh_matrix(t_mesh *mesh)
{
	float	translation_mat[4][4];
	float	tmp[4][4];

	ident_mat_4x4(mesh->mesh_matrix);
	// ident_mat_4x4(tmp);
	// mat4x4_mult_mat4x4(mesh->rotation_mat_z, mesh->rotation_mat_x, tmp);
//	translation_matrix(translation_mat, 0, 0, 0);
	//mat4x4_mult_mat4x4(tmp, translation_mat, mesh->mesh_matrix);
}

t_light	init_day_light(double d_time)
{
	t_light			day_light;
	static t_vec3	direct =  {.x = 1.0f, .y = -1.0f, .z = -1.0f, .w = 0};
	static float light_direct = -1;

	if (direct.x < -1)
		light_direct = 1;
	if (direct.x > 1)
		light_direct = -1;
	direct.x += d_time * light_direct / 50;
	float day_progress = (direct.x + 1) / 2;
	static float timer = 0;
	timer += d_time;
	if (timer > 1)
	{
		//fprintf(stderr, "day time: %f\n", 24 * day_progress);
		timer = 0;
	}
	float	light_intens = 1 - fabs(0.5 - day_progress);
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

	// printf("full size: %zu\nsize: %zu\n", sizeof(t_triangle), (uintptr_t)(&src->unprojected_z) - (uintptr_t)src);
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
	const float	project_mat[4][4] = PROJECTION_MATRIX;

	clipped_count_front = clipping_z_near(&triangle, clipped_z_front);
	int	j;
	int	q;
	j = 0;
	while (j == 0 || (j < clipped_count_front && j < 2))
	{
		if (clipped_count_front)
			triangle = clipped_z_front[j];
		clipped_count_back = clipping_z_far(&triangle, clipped_z_back);
		q = 0;
		uint32_t col = clipped_z_front[j].col; //TODO remove this line after fixing/implenting clipping
		while (q == 0 || (q < clipped_count_back && q < 2))
		{
			if (clipped_count_back)
				triangle = clipped_z_back[q];

			// enter projeceted space
			//cpy_triangle_data_rasterize(base_data, &projected);
			projected.col = col; //TODO remove this line after fixing/implenting clipping
			matrix_mult_vec3_4x4(triangle.p + 0, project_mat, &projected.p[0]);
			matrix_mult_vec3_4x4(triangle.p + 1, project_mat, &projected.p[1]);
			matrix_mult_vec3_4x4(triangle.p + 2, project_mat, &projected.p[2]);
			cpy_triangle_data_rasterize(base_data, &projected);
			if (!zero_f(projected.p[0].w))
				div_vec3(projected.p + 0, projected.p[0].w);
			if (!zero_f(projected.p[1].w))
				div_vec3(projected.p + 1, projected.p[1].w);
			if (!zero_f(projected.p[2].w))
				div_vec3(projected.p + 2, projected.p[2].w);

			scale_to_screen(&projected);

			projected.unprojected_z[0] = triangle.p[0].z;
			projected.unprojected_z[1] = triangle.p[1].z;
			projected.unprojected_z[2] = triangle.p[2].z;
			t_triangle				clipped[30];
			clipped[0] = projected;
			int clipped_count = call_clipping_xy(clipped);

			int l = 0;
			while (l < clipped_count)
			{
				projected = clipped[l];
				cpy_triangle_data_rasterize(base_data, &projected);
				for (int i = 0; i < 3; i++)
				{
					if (projected.p[i].x < 0.0f || projected.p[i].x >= (float)WIDTH)// || p[i].y < 0 ||  p[i].y >= HEIGHT)
					{
						printf("error before filling: clipping error or trash memory, l: %d clipped count: %d\n", l, clipped_count);
						print_vec3(projected.p[0], 0);
						print_vec3(projected.p[1], 0);
						print_vec3(projected.p[2], 0);
						//exit(1);
					}
				}
				
				{
					if (!projected.p[0].mtl || !projected.p[0].mtl->texture)
						fill_triangle_color(mesh->img, &projected, base_data->col, mesh);
					else
						fill_triangle_texture(mesh->img, &projected, mesh, color_scalars);
				}
				l++;
			}
			q++;
		}
		j++;
	}
}


void	draw_mesh(t_mesh *mesh)
{
	int			i;
	t_triangle		transformed;
	t_triangle		viewed;

	i = 0;
	static double	theta = 0;
		theta += *mesh->d_time;
	t_vec3	traveled_dist;
	traveled_dist.x = mesh->momentum.x * *mesh->d_time;
	traveled_dist.y = mesh->momentum.y * *mesh->d_time;
	traveled_dist.z = mesh->momentum.z * *mesh->d_time;
	#ifdef MOVEMENT
	if (mesh->triangles != mesh->main->axis.triangles)
		translate_mesh_3d(mesh, traveled_dist);
	#endif
	fill_mesh_matrix(mesh);

	t_vec3	vec_target = v3_add(mesh->main->camera, mesh->main->look_direct);
	float	camera[4][4];
	matrix_point_at(&mesh->main->camera, &vec_target, &mesh->main->up, camera);
	float	mat_view[4][4];
	matrix_look_at(camera, mat_view);
	

	t_light	day_light;

	day_light = init_day_light(*mesh->d_time);


	t_light			ambient_light2;
	const t_vec3	light_direct2 =  {1.0f, -1.0f, -1.0f};
	init_light(&ambient_light2, light_direct2, WHITE, 0.0f);

	// t_light			ambient_light3;
	// const t_vec3	light_direct3 =  {-1.0f, 1.0f, -1.0f};
	// init_light(&ambient_light3, light_direct3, WHITE);
	t_vec3	bounds_result;
	while (i < mesh->count)
	{

		transformed = mesh->triangles[i];//not neededm, for debugging
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 0, mesh->mesh_matrix, transformed.p + 0);
		matrix_mult_vec3_4x4 (mesh->triangles[i].p + 1, mesh->mesh_matrix, transformed.p + 1);
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 2, mesh->mesh_matrix, transformed.p + 2);

		// t_vec3	tmp = transformed.normal;
		// float	tmp_mat[4][4];
		// mat4x4_mult_mat4x4(mesh->rotation_mat_z, mesh->rotation_mat_x, tmp_mat);
		// matrix_mult_vec3_4x4(&tmp, tmp_mat, &transformed.normal);




		bounds_result = out_of_bound_triangle(&transformed);
		if (bounds_result.z == -3 || bounds_result.z == 3)
		{
			i++;
			continue ;
		}
		//if (!mesh->obj_file)
		{
			transformed.normal = cross_product(v3_sub(transformed.p[1], transformed.p[0]), v3_sub(transformed.p[2], transformed.p[0]));
		}
		unit_vec3(&transformed.normal);
		if (dot_prod_unit(transformed.normal, v3_sub(transformed.p[0], mesh->main->camera)) >= 0)
		//if (dot_prod_unit(transformed.normal, v3_sub(transformed.p[0], mesh->main->camera)) < 0)
		{
			i++;
			continue ;
		}

		t_light_argb_stren	color_scalars = {0};

		float light_dp = dot_prod_unit(transformed.normal, day_light.direct);
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

		viewed = transformed;
		// enter view space
		matrix_mult_vec3_4x4(transformed.p + 0, mat_view, viewed.p + 0);
		matrix_mult_vec3_4x4 (transformed.p + 1, mat_view, viewed.p + 1);
		matrix_mult_vec3_4x4(transformed.p + 2, mat_view, viewed.p + 2);

		rasterize(viewed, mesh, mesh->triangles + i, color_scalars);
		i++;
	}
}
