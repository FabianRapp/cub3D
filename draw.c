
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
		printf("x: %d y: %d\n", pos[X], pos[Y]);
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

bool	zero_f(float f)
{
	if (fabs(f) < 0.01)
	{
		printf("zero\n");
		return (true);
	}
	return (false);
}

void	fill_triangle(mlx_image_t *img, t_triangle *projected, uint32_t color, t_mesh *mesh)
{
	t_vec3	*p = projected->p;
	float		*depth;

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
	float	cur_max_y = p[1].y;
	float	total_y_progress;
	int		cur_x;
	static int i = 0;
	if (cur_y_float < 0.0f)
	{
		cur_y_float = 0.0f;
	}
	if (!((p[0].x < 0 && p[1].x < 0) || (p[0].x >= WIDTH && p[1].x >= WIDTH) || (p[0].y < 0 && p[1].y < 0) || (p[0].y >= HEIGHT && p[1].y >= HEIGHT) || (p[0].z < Z_NEAR && p[1].z < Z_NEAR) || (projected->unprojected_z[0] >= Z_FAR && projected->unprojected_z[1] >= Z_FAR)))
	{
		int y_index =  (int)roundf(cur_y_float);
		while (cur_y_float < cur_max_y && y_index < HEIGHT)
		{
			total_y_progress = (cur_y_float - p[0].y) / (p[2].y - p[0].y);
			cur_x = (int)roundf((p[2].x - p[0].x) * total_y_progress + p[0].x);
			float	y_progress =  (cur_y_float - p[0].y) / (cur_max_y - p[0].y);
			int	x_max = (int)roundf((p[1].x - p[0].x) * y_progress + p[0].x);
			float cur_z;
			float start_z = y_progress * (projected->unprojected_z[1] - projected->unprojected_z[0]) + projected->unprojected_z[0];
			float end_z = total_y_progress * (projected->unprojected_z[2] - projected->unprojected_z[0]) + projected->unprojected_z[0];
			if (p[1].x > p[2].x)
			{
				float tmp;
				tmp = start_z;
				start_z = end_z;
				end_z = start_z;
			}
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
			if (cur_x <= x_max)
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
						ft_put_pixel_fin_index(img->pixels, fin_index, color);
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
						ft_put_pixel_fin_index(img->pixels, fin_index, color);
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
	}
	if (!((p[1].x < 0 && p[2].x < 0) || (p[1].x >= WIDTH && p[2].x >= WIDTH) || (p[1].y < 0 && p[2].y < 0) || (p[1].y >= HEIGHT && p[2].y >= HEIGHT) || (p[1].z < Z_NEAR && p[2].z < Z_NEAR) || (projected->unprojected_z[1] >= Z_FAR && projected->unprojected_z[2] >= Z_FAR)))
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
			if (p[1].x > p[2].x)
			{
				float tmp;
				tmp = start_z;
				start_z = end_z;
				end_z = start_z;
			}
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
			if (cur_x <= x_max)
			{
				if (cur_x < 0)
					cur_x = 0;
				while(cur_x <= x_max)
				{
					float x_progress = (cur_x - start_x) / len_x;
					cur_z = x_progress * z_dist + start_z;
					if (cur_x >= 0 && cur_x < WIDTH)
					{
						int fin_index = cur_x + row_index;
						if (cur_z > Z_NEAR && cur_z < depth[fin_index])
						{
							depth[fin_index] = cur_z;
							ft_put_pixel_fin_index(img->pixels, fin_index, color);
						}
					}
					cur_x++;
				}
			}
			else if (cur_x > x_max)
			{
				if (cur_x >= WIDTH)
					cur_x = WIDTH - 1;
				while (cur_x >= x_max)
				{
					float x_progress = (cur_x - start_x) / len_x;
					cur_z = x_progress * z_dist + start_z;
					if (cur_x >= 0 && cur_x < WIDTH && y_index >= 0 && y_index < HEIGHT)
					{
						int fin_index = cur_x + row_index;
						if (cur_z > Z_NEAR && cur_z < depth[fin_index])
						{
							depth[fin_index] = cur_z;
							ft_put_pixel_fin_index(img->pixels, fin_index, color);
						}
					}
					cur_x--;
				}
			}
			cur_y_float += 1.0f;
			y_index =  (int)roundf(cur_y_float);
		}
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
	norm_vec3(&light->direct);
	light->color.col = color;
	//light->strength.v[A] = light->color.argb[A] / (0xFF * base_stren);
	light->strength.v[R] = light->color.argb[R] / (0xFF * base_stren);
	light->strength.v[G] = light->color.argb[G] / (0xFF * base_stren);
	light->strength.v[B] = light->color.argb[B] / (0xFF * base_stren);
}

t_triangle	apply_rotation_addtiononal_translation(t_mesh *mesh, int i)
{
	t_triangle		rotated_xz;
	t_triangle		rotated_xyz;
	t_triangle		rotated_z;
	t_triangle		translated;

	// printf("p1 x: %f, y: %f z: %f\n", mesh->triangles[i].p[0].x, mesh->triangles[i].p[0].y, mesh->triangles[i].p[0].z);
	// printf("p2 x: %f, y: %f z: %f\n", mesh->triangles[i].p[1].x, mesh->triangles[i].p[1].y, mesh->triangles[i].p[1].z);
	//printf("p3 x: %f, y: %f z: %f\n\n", mesh->triangles[i].p[2].x, mesh->triangles[i].p[2].y, mesh->triangles[i].p[2].z);
	matrix_mult_vec3_4x4((mesh->triangles + i)->p + 0, mesh->rotation_mat_z, &rotated_z.p[0]);
	matrix_mult_vec3_4x4((mesh->triangles + i)->p + 1, mesh->rotation_mat_z, &rotated_z.p[1]);
	matrix_mult_vec3_4x4((mesh->triangles + i)->p + 2, mesh->rotation_mat_z, &rotated_z.p[2]);
	if (mesh->obj_file)
	{
		matrix_mult_vec3_4x4(&(mesh->triangles + i)->normal, mesh->rotation_mat_z, &rotated_z.normal);
		//matrix_mult_vec3_4x4((mesh->triangles + i)->obj_normal + 0, mesh->rotation_mat_z, &rotated_z.obj_normal[0]);
		//matrix_mult_vec3_4x4((mesh->triangles + i)->obj_normal + 1, mesh->rotation_mat_z, &rotated_z.obj_normal[1]);
		//matrix_mult_vec3_4x4((mesh->triangles + i)->obj_normal + 2, mesh->rotation_mat_z, &rotated_z.obj_normal[2]);
	}
	// printf("p1 x: %f, y: %f z: %f\n", rotated_z.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
	// printf("p2 x: %f, y: %f z: %f\n", rotated_z.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
	// printf("p3 x: %f, y: %f z: %f\n\n", rotated_z.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);
	matrix_mult_vec3_4x4(rotated_z.p + 0, mesh->rotation_mat_x, &rotated_xz.p[0]);
	matrix_mult_vec3_4x4(rotated_z.p + 1, mesh->rotation_mat_x, &rotated_xz.p[1]);
	matrix_mult_vec3_4x4(rotated_z.p + 2, mesh->rotation_mat_x, &rotated_xz.p[2]);
	if (mesh->obj_file)
	{
		matrix_mult_vec3_4x4(&rotated_z.normal, mesh->rotation_mat_x, &rotated_xz.normal);
		//matrix_mult_vec3_4x4(rotated_z.obj_normal + 0, mesh->rotation_mat_x, &rotated_xz.obj_normal[0]);
		//matrix_mult_vec3_4x4(rotated_z.obj_normal + 1, mesh->rotation_mat_x, &rotated_xz.obj_normal[1]);
		//matrix_mult_vec3_4x4(rotated_z.obj_normal + 2, mesh->rotation_mat_x, &rotated_xz.obj_normal[2]);
	}
	matrix_mult_vec3_4x4(rotated_xz.p + 0, mesh->rotation_mat_y, &rotated_xyz.p[0]);
	matrix_mult_vec3_4x4(rotated_xz.p + 1, mesh->rotation_mat_y, &rotated_xyz.p[1]);
	matrix_mult_vec3_4x4(rotated_xz.p + 2, mesh->rotation_mat_y, &rotated_xyz.p[2]);
	if (mesh->obj_file)
	{
		matrix_mult_vec3_4x4(&rotated_xz.normal, mesh->rotation_mat_y, &rotated_xyz.normal);
		//matrix_mult_vec3_4x4(rotated_xz.obj_normal + 0, mesh->rotation_mat_y, &rotated_xyz.obj_normal[0]);
		//matrix_mult_vec3_4x4(rotated_xz.obj_normal + 1, mesh->rotation_mat_y, &rotated_xyz.obj_normal[1]);
		//matrix_mult_vec3_4x4(rotated_xz.obj_normal + 2, mesh->rotation_mat_y, &rotated_xyz.obj_normal[2]);
	}
	// printf("p1 x: %f, y: %f z: %f\n", rotated_xz.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
	// printf("p2 x: %f, y: %f z: %f\n", rotated_xz.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
	// printf("p3 x: %f, y: %f z: %f\n\n", rotated_xz.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);
	translated = rotated_xyz;
	translated.p[0].z += 10.0f;
	translated.p[1].z += 10.0f;
	translated.p[2].z += 10.0f;



	return (translated);

}

void	draw_mesh(t_mesh *mesh)
{
	int			i;

	t_triangle		translated;
	t_triangle		projected;
	t_color_split	color;



	int			p_2d[3][2];
	const float	project_mat[4][4] = PROJECTION_MATRIX;
	i = 0;
	static double	theta = 0;
		theta += *mesh->d_time;
	t_vec3	traveled_dist;
	traveled_dist.x = mesh->momentum.x * *mesh->d_time;
	traveled_dist.y = mesh->momentum.y * *mesh->d_time;
	traveled_dist.z = mesh->momentum.z * *mesh->d_time;
	#ifdef MOVEMENT
		translate_mesh_3d(mesh, traveled_dist);
	#endif
	//print_vec3(traveled_dist, "traveled_dist: ");
	bool flipped_x = false;
	bool flipped_y = false;
	bool flipped_z = false;
	// mesh->center.x = 0;
	// mesh->center.y = 0;
	// mesh->center.z = 0;
	t_light			ambient_light1;
	static t_vec3	light_direct1 =  {1.0f, 1.0f, -1.0f};
	static int light_direct = -1;
	if (light_direct1.x < -1)
		light_direct = 1;
	if (light_direct1.x > 1)
		light_direct = -1;
	light_direct1.x += *mesh->d_time * light_direct / 50;
	float day_progress = (light_direct1.x + 1) / 2;
	static float timer = 0;
	timer += *mesh->d_time;
	if (timer > 1)
	{
		printf("day time: %f\n", 24 * day_progress);
		timer = 0;
	}
	float	light_intens = 1 - fabs(0.5 - day_progress);
	t_color_split light_col;
	light_col.col = WHITE;
	light_col.argb[R] *= 1 - 0.3 * fabs(0.5 - day_progress);
	light_col.argb[G] *= 1 - 0.6 * fabs(0.5 - day_progress);
	light_col.argb[B] *= 1 - fabs(0.5 - day_progress);
	init_light(&ambient_light1, light_direct1, light_col.col, 1 - fabs(0.5 - day_progress));


	t_light			ambient_light2;
	const t_vec3	light_direct2 =  {1.0f, -1.0f, -1.0f};
	init_light(&ambient_light2, light_direct2, WHITE, 0.0f);

	// t_light			ambient_light3;
	// const t_vec3	light_direct3 =  {-1.0f, 1.0f, -1.0f};
	// init_light(&ambient_light3, light_direct3, WHITE);
	while (i < mesh->count)
	{
		color.col = (mesh->triangles + i)->col;


		translated = apply_rotation_addtiononal_translation(mesh, i);

		translated.normal = cross_product(v3_sub(translated.p[1], translated.p[0]), v3_sub(translated.p[2], translated.p[0]));
		float normal_len = sqrtf(translated.normal.x * translated.normal.x + translated.normal.y * translated.normal.y + translated.normal.z * translated.normal.z);
		scale_vec3(&translated.normal, 1 / normal_len);
		if (dot_prod(translated.normal, v3_sub(translated.p[0], mesh->main->camera)) >= 0)
		{
			i++;
			continue ;
		}
	
		t_light_argb_stren	color_scalars = {0};
	
		float light_dp = dot_prod(translated.normal, ambient_light1.direct);
		light_dp = fmaxf(light_dp, 0.0f);
		color_scalars.v[R] += ambient_light1.strength.v[R] *  light_dp;
		color_scalars.v[G] += ambient_light1.strength.v[G] *  light_dp;
		color_scalars.v[B] += ambient_light1.strength.v[B] *  light_dp;

		// light_dp = dot_prod(translated.normal, ambient_light2.direct);
		// light_dp = fmaxf(light_dp, 0.0f);
		// color_scalars.v[R] += ambient_light2.strength.v[R] *  light_dp;
		// color_scalars.v[G] += ambient_light2.strength.v[G] *  light_dp;
		// color_scalars.v[B] += ambient_light2.strength.v[B] *  light_dp;

		// light_dp = dot_prod(translated.normal, ambient_light3.direct);
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


		matrix_mult_vec3_4x4(translated.p + 0, project_mat, &projected.p[0]);
		matrix_mult_vec3_4x4(translated.p + 1, project_mat, &projected.p[1]);
		matrix_mult_vec3_4x4(translated.p + 2, project_mat, &projected.p[2]);

		// printf("p1 x: %f, y: %f z: %f\n", translated.p[0].x, translated.p[0].y, translated.p[0].z);
		// printf("p2 x: %f, y: %f z: %f\n", projected.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
		// printf("p3 x: %f, y: %f z: %f\n\n", projected.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);

		// scale the points
		projected.p[0].x += 1.0f;
		projected.p[0].y += 1.0f;
		projected.p[1].x += 1.0f;
		projected.p[1].y += 1.0f;
		projected.p[2].x += 1.0f;
		projected.p[2].y += 1.0f;

		projected.p[0].x *= 0.5f * (float)WIDTH;
		projected.p[0].y *= 0.5f * (float)HEIGHT;
		projected.p[1].x *= 0.5f * (float)WIDTH;
		projected.p[1].y *= 0.5f * (float)HEIGHT;
		projected.p[2].x *= 0.5f * (float)WIDTH;
		projected.p[2].y *= 0.5f * (float)HEIGHT;

		p_2d[0][X] = (int)roundf(projected.p[0].x);
		p_2d[0][Y] = (int)roundf(projected.p[0].y);
		p_2d[1][X] = (int)roundf(projected.p[1].x);
		p_2d[1][Y] = (int)roundf(projected.p[1].y);
		p_2d[2][X] = (int)roundf(projected.p[2].x);
		p_2d[2][Y] = (int)roundf(projected.p[2].y);
		projected.unprojected_z[0] = translated.p[0].z;
		projected.unprojected_z[1] = translated.p[1].z;
		projected.unprojected_z[2] = translated.p[2].z;
		t_vec3	bounds_result = out_of_bound_triangle(&projected);
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
		fill_triangle(mesh->img, &projected, color.col, mesh);
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
	
		float normal_len = sqrtf(base->normal.x * base->normal.x + base->normal.y * base->normal.y + base->normal.z * base->normal.z);
		scale_vec3(&base->normal, 1 / normal_len);
		if (dot_prod(base->normal, v3_sub(base->p[0], mesh->main->camera)) < 0)
		//if (base->normal.z > 0)
		{
			i++;
			continue ;
		}
		matrix_mult_vec3_4x4(base->p + 0, project_mat, &projected.p[0]);
		matrix_mult_vec3_4x4(base->p + 1, project_mat, &projected.p[1]);
		matrix_mult_vec3_4x4(base->p + 2, project_mat, &projected.p[2]);

		// scale the points
		projected.p[0].x += 1.0f;
		projected.p[0].y += 1.0f;
		projected.p[1].x += 1.0f;
		projected.p[1].y += 1.0f;
		projected.p[2].x += 1.0f;
		projected.p[2].y += 1.0f;

		projected.p[0].x *= 0.5f * (float)WIDTH;
		projected.p[0].y *= 0.5f * (float)HEIGHT;
		projected.p[1].x *= 0.5f * (float)WIDTH;
		projected.p[1].y *= 0.5f * (float)HEIGHT;
		projected.p[2].x *= 0.5f * (float)WIDTH;
		projected.p[2].y *= 0.5f * (float)HEIGHT;

		projected.unprojected_z[0] = base->p[0].z;
		projected.unprojected_z[1] = base->p[1].z;
		projected.unprojected_z[2] = base->p[2].z;
		//draw_triangle(mesh->img, &projected, (mesh->triangles + i)->col);
		fill_triangle(mesh->img, &projected, (mesh->triangles + i)->col, mesh);
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