/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 01:39:06 by frapp             #+#    #+#             */
/*   Updated: 2024/04/28 16:02:18 by frapp            ###   ########.fr       */
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
	// if (!dist_x + dist_y)
	// {
	// 	printf("error no dist\n");
	// 	if (color == RED)
	// 		printf("red\n");
	// 	else if (color == GREEN)
	// 		printf("green\n");
	// 	else if (color == BLUE)
	// 		printf("blue\n");
	// 	else
	// 		printf("other color\n");
	//}
	// if (color == RED)// && dist_x + dist_y)
	// {
	// 	printf("target x: %d target y: %d\n", x2, y2);
	// 	printf("pos x: %d pos y: %d\n", pos[X], pos[Y]);
	// 	//printf("red dist: %d\n", dist_x + dist_y);
	// }
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
					//mlx_put_pixel(image, pos[X], pos[Y], color);
				//else
				//	printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
				pos[X] += direct_x;
			}
		}
		if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
			ft_put_pixel(image->pixels, pos[X], pos[Y], color);
			//mlx_put_pixel(image, pos[X], pos[Y], color);
		//else
			//printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
		return ;
	}

	last_error = dist_x - dist_y;
	while (pos[X] != x2 || pos[Y] != y2)
	{
		//printf("x: %d y: %d\n", pos[X], pos[Y]);
		if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
			ft_put_pixel(image->pixels, pos[X], pos[Y], color);
			//mlx_put_pixel(image, pos[X], pos[Y], color);
	//	else
			//printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
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
		//printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
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
	printf("total color: %x\n", col.col);
	printf("a: %x r: %x g: %x b: %x\n", col.argb[A], col.argb[R], col.argb[G], col.argb[B]);
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
		//printf("day time: %f\n", 24 * day_progress);
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

void	draw_mesh(t_mesh *mesh)
{
	int			i;
	t_triangle		transformed;
	t_triangle		projected;
	t_triangle		viewed;
	t_color_split	color;

	const float	project_mat[4][4] = PROJECTION_MATRIX;
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
	//
	fill_mesh_matrix(mesh);

	t_vec3	vec_target = v3_add(mesh->main->camera, mesh->main->look_direct);
	float	camera[4][4];
	matrix_point_at(&mesh->main->camera, &vec_target, &mesh->main->up, camera);
	float	mat_view[4][4];
	matrix_look_at(camera, mat_view);
	
	//mat4x4_mult_mat4x4( , ,main_data->world_mat);
	//print_vec3(traveled_dist, "traveled_dist: ");
	bool flipped_x = false;
	bool flipped_y = false;
	bool flipped_z = false;
	// mesh->center.x = 0;
	// mesh->center.y = 0;
	// mesh->center.z = 0;

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
		color.col = (mesh->triangles + i)->col;

		transformed = mesh->triangles[i];//not neededm, for debugging
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 0, mesh->mesh_matrix, transformed.p + 0);
		matrix_mult_vec3_4x4 (mesh->triangles[i].p + 1, mesh->mesh_matrix, transformed.p + 1);
		matrix_mult_vec3_4x4(mesh->triangles[i].p + 2, mesh->mesh_matrix, transformed.p + 2);

		// div_vec3(transformed.p + 0, transformed.p[0].w);
		// div_vec3(transformed.p + 1, transformed.p[1].w);
		// div_vec3(transformed.p + 2, transformed.p[2].w);


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

		color_scalars.v[R] = fmin(color_scalars.v[R], 1.0f);
		color_scalars.v[G] = fmin(color_scalars.v[G], 1.0f);
		color_scalars.v[B] = fmin(color_scalars.v[B], 1.0f);

		color.argb[R] *= color_scalars.v[R];
		color.argb[G] *= color_scalars.v[G];
		color.argb[B] *= color_scalars.v[B];

		viewed = transformed;
		// enter view space
		matrix_mult_vec3_4x4(transformed.p + 0, mat_view, viewed.p + 0);
		matrix_mult_vec3_4x4 (transformed.p + 1, mat_view, viewed.p + 1);
		matrix_mult_vec3_4x4(transformed.p + 2, mat_view, viewed.p + 2);


		// enter projeceted space
		ft_memcpy(&projected, mesh->triangles + i, sizeof(projected));
		// print_vec3(viewed.p[0], "0: ");
		// print_vec3(viewed.p[1], "1: ");
		// print_vec3(viewed.p[2], "2: ");
		matrix_mult_vec3_4x4(viewed.p + 0, project_mat, &projected.p[0]);
		matrix_mult_vec3_4x4(viewed.p + 1, project_mat, &projected.p[1]);
		matrix_mult_vec3_4x4(viewed.p + 2, project_mat, &projected.p[2]);

		if (!zero_f(projected.p[0].w))
			div_vec3(projected.p + 0, projected.p[0].w);
		if (!zero_f(projected.p[1].w))
			div_vec3(projected.p + 1, projected.p[1].w);
		if (!zero_f(projected.p[2].w))
			div_vec3(projected.p + 2, projected.p[2].w);
	
		// printf("p1 x: %f, y: %f z: %f\n", transformed.p[0].x, transformed.p[0].y, transformed.p[0].z);
		// printf("p2 x: %f, y: %f z: %f\n", projected.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
		// printf("p3 x: %f, y: %f z: %f\n\n", projected.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);

		scale_to_screen(&projected);

		projected.unprojected_z[0] = viewed.p[0].z;
		projected.unprojected_z[1] = viewed.p[1].z;
		projected.unprojected_z[2] = viewed.p[2].z;
		bounds_result = out_of_bound_triangle_projeceted(&projected);
		if (!flipped_x && bounds_result.x < 0 && mesh->momentum.x < 0)
		{
			mesh->momentum.x *= -1;
			flipped_x = true;
		}
		if (!flipped_x && bounds_result.x > 0 && mesh->momentum.x > 0)
		{
			mesh->momentum.x *= -1;
			flipped_x = true;
		}

		if (!flipped_y && bounds_result.y < 0 && mesh->momentum.y < 0)
		{
			mesh->momentum.y *= -1;
			flipped_y = true;
		}
		if (!flipped_y && bounds_result.y > 0 && mesh->momentum.y > 0)
		{
			mesh->momentum.y *= -1;
			flipped_y = true;
		}

		// if (!flipped_z && bounds_result.z < 0 && mesh->momentum.z < 0)
		// {
		// 	mesh->momentum.z *= -1;
		// 	flipped_z = true;
		// }
		// if (!flipped_z && bounds_result.z > 0 && mesh->momentum.z > 0)
		// {
		// 	mesh->momentum.z *= -1;
		// 	flipped_z = true;
		// }
		//draw_triangle(mesh->img, &projected, (mesh->triangles + i)->col);
		if (bounds_result.x < 3 && bounds_result.x > -3 && bounds_result.y < 3 && bounds_result.y > -3)
		{
			if (!projected.p[0].mtl || !projected.p[0].mtl->texture)
				fill_triangle_color(mesh->img, &projected, color.col, mesh);
			else
				fill_triangle_texture(mesh->img, &projected, mesh, color_scalars);
		}
		i++;
	//	if (i == 2640)
		//	break ;
		
	}
	//mesh->center = v3_scale(mesh->center, 1.0f / (float) mesh->count);
	// mesh->center = length_vec3(&mesh->center) / mesh->count;
	// if (done)
	// {
		// float len = length_vec3(&mesh->center);
		// t_vec3 normalized = v3_scale(mesh->center, 1.0f / (float) len);
		// // mesh->momentum = v3_add(mesh->momentum, v3_reverse(normalized));
		// mesh->momentum = v3_reverse(normalized);
		// mesh->momentum.z += 1.0f;
		// double a = 10.0;
		// mesh->momentum.z = modf(50.0, (double *)(mesh->momentum.p + Z));
		//scale_vec3(&mesh->momentum, 1);
//	}
	//if (mesh->triangles->col == GREEN)
		//print_vec3(mesh->center, "\n");
	
}

void	draw_skybox(t_mesh *mesh)
{
	int			i;

	t_triangle	projected;
	t_triangle	*base;
	const float	project_mat[4][4] = PROJECTION_MATRIX;
	i = 0;
	while (i < mesh->count)
	{
		base = mesh->triangles + i;
		base->normal = cross_product(v3_sub(base->p[1], base->p[0]), v3_sub(base->p[2], base->p[0]));
	

		
		// unit_vec3(&base->normal);
		// if (dot_prod_unit(base->normal, v3_sub(base->p[0], mesh->main->camera)) <= 0)

		//float normal_len = sqrtf(base->normal.x * base->normal.x + base->normal.y * base->normal.y + base->normal.z * base->normal.z);
		//scale_vec3(&base->normal, 1 / normal_len);
		// if (dot_prod_unit(base->normal, v3_sub(base->p[0], mesh->main->camera)) >= 0)
		// //if (base->normal.z > 0)
		// {
		// 	i++;
		// 	continue ;
		// }
		matrix_mult_vec3_4x4(base->p + 0, project_mat, &projected.p[0]);
		matrix_mult_vec3_4x4(base->p + 1, project_mat, &projected.p[1]);
		matrix_mult_vec3_4x4(base->p + 2, project_mat, &projected.p[2]);

		scale_to_screen(&projected);

		projected.unprojected_z[0] = base->p[0].z;
		projected.unprojected_z[1] = base->p[1].z;
		projected.unprojected_z[2] = base->p[2].z;
		//draw_triangle(mesh->img, &projected, (mesh->triangles + i)->col);
		fill_triangle_color(mesh->img, &projected, (mesh->triangles + i)->col, mesh);
		i++;
	}
	//mesh->center = v3_scale(mesh->center, 1.0f / (float) mesh->count);
	// mesh->center = length_vec3(&mesh->center) / mesh->count;
	// if (done)
	// {
		// float len = length_vec3(&mesh->center);
		// t_vec3 normalized = v3_scale(mesh->center, 1.0f / (float) len);
		// // mesh->momentum = v3_add(mesh->momentum, v3_reverse(normalized));
		// mesh->momentum = v3_reverse(normalized);
		// mesh->momentum.z += 1.0f;
		// double a = 10.0;
		// mesh->momentum.z = modf(50.0, (double *)(mesh->momentum.p + Z));
		//scale_vec3(&mesh->momentum, 1);
//	}
	//if (mesh->triangles->col == GREEN)
		//print_vec3(mesh->center, "\n");
	
}