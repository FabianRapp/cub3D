
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
	pos[X] = x1;
	pos[Y] = y1;
	direct_x = (pos[X] > x2) * -1;
	direct_y = (pos[Y] > y2) * -1;
	if (!direct_x)
		direct_x++;
	if (!direct_y)
		direct_y++;
	// handle vert/hor lines extra
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

void	sort_vertexes_for_y(t_vec3 p[3])
{
	t_vec3	tmp;

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

float	slope_2d_x_per_y(t_vec3 p1, t_vec3 p2)
{
	if (p1.y == p2.y)
		return (0.0f);
	return ((p2.x - p1.x) / (p2.y - p1.y));
}

void	fill_triangle(mlx_image_t *img, t_triangle *projected, uint32_t color)
{
	t_vec3	*p = projected->p;
	
	sort_vertexes_for_y(p);
	if (!(p[0].y <= p[1].y && p[1].y <= p[2].y))
	{
		printf("error sort_vertexes_for_y 1: %f 2: %f 3: %f\n", p[0].y, p[1].y, p[2].y);
		exit(1);
	}
	
	float	m1 = slope_2d_x_per_y(p[0], p[1]);
	float	m2 = slope_2d_x_per_y(p[0], p[2]);
	float	m3 = slope_2d_x_per_y(p[1], p[2]);
	float	y_dist1 = p[1].y - p[0].y;
	float	cur_y = p[0].y;
	float	cur_max_y = p[1].y;
	float	total_y_progress;
	int	cur_x;
	int	y_index;
	static int i = 0;
	if (cur_y < 0)
		cur_y = 0;
	y_index = (int)(roundf(cur_y));
	while (cur_y < cur_max_y && y_index < HEIGHT)
	{
		total_y_progress = (cur_y - p[0].y) / (p[2].y - p[0].y);
		cur_x = (int)(roundf((p[2].x - p[0].x) * total_y_progress + p[0].x));
		float	y_progress =  (cur_y - p[0].y) / (cur_max_y - p[0].y);
		int	x_max = (int)roundf((p[1].x - p[0].x) * y_progress + p[0].x);
		
		if (cur_x <= x_max)
		{
			if (cur_x < 0)
				cur_x = 0;
			while (cur_x <= x_max && cur_x < WIDTH)
			{
				ft_put_pixel(img->pixels, cur_x, y_index, color);
				cur_x++;
			}
		}
		else if (cur_x > x_max)
		{
			if (cur_x >= WIDTH)
				cur_x = WIDTH - 1;
			while (cur_x >= x_max && cur_x > 0)
			{
				ft_put_pixel(img->pixels, cur_x, y_index, color);
				cur_x--;
			}
		}
		cur_y += 1;
		y_index = (int)(roundf(cur_y));
	}
	
	int x_max;
	y_index = (int)(roundf(cur_y));
	while (cur_y <= p[2].y && y_index < HEIGHT)
	{
		
		float	y_progress =  (cur_y - p[1].y) / (p[2].y - p[1].y);
		cur_x = m3 *  (cur_y - p[1].y) + p[1].x;
		x_max = m2 * (cur_y - p[0].y) + p[0].x;
		y_progress = (cur_y - p[1].y) / (p[2].y - p[1].y);
		total_y_progress = (cur_y - p[0].y) / (p[2].y - p[0].y);
		if (cur_x <= x_max)
		{
			//printf("cur_x : %d x_max: %d target x: %f y_progress: %f\n", cur_x, x_max, p[1].x, y_progress);
			while(cur_x < x_max)
			{
				if (cur_x >= 0 && cur_x < WIDTH && y_index >= 0 && y_index < HEIGHT)
					ft_put_pixel(img->pixels, cur_x, y_index, color);
				cur_x++;
			}
		}
		else if (cur_x > x_max)
		{
			//printf("cur_x : %d x_max: %d target x: %f y_progress: %f\n", cur_x, x_max, p[1].x, y_progress);
			while (cur_x >= x_max)
			{
				if (cur_x >= 0 && cur_x < WIDTH && y_index >= 0 && y_index < HEIGHT)
					ft_put_pixel(img->pixels, cur_x, y_index, color);
				cur_x--;
			}
		}
		cur_y += 1;
		y_index = (int)(roundf(cur_y));
	}
}

void	draw_triangle(mlx_image_t *img, t_triangle *projected, uint32_t color)
{
	draw_line(img, (int)roundf(projected->p->x), (int)roundf(projected->p[1].x), (int)roundf(projected->p->y), (int)roundf(projected->p[1].y), color);
	draw_line(img, (int)roundf(projected->p[1].x), (int)roundf(projected->p[2].x), (int)roundf(projected->p[1].y), (int)roundf(projected->p[2].y), color);
	draw_line(img, (int)roundf(projected->p[2].x), (int)roundf(projected->p->x), (int)roundf(projected->p[2].y), (int)roundf(projected->p->y), color);
}

void	draw_cube(t_mesh *mesh)
{
	int			i;
	t_triangle	rotated_xz;
	t_triangle	rotated_xyz;
	t_triangle	rotated_z;
	t_triangle	translated;
	t_triangle	projected;

	int			p_2d[3][2];
	const float	project_mat[4][4] = PROJECTION_MATRIX;
	i = 0;
	ft_bzero(mesh->img->pixels, 4 * mesh->img->height * mesh->img->width);
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
	while (i < mesh->count)
	{
		// printf("p1 x: %f, y: %f z: %f\n", mesh->triangles[i].p[0].x, mesh->triangles[i].p[0].y, mesh->triangles[i].p[0].z);
		// printf("p2 x: %f, y: %f z: %f\n", mesh->triangles[i].p[1].x, mesh->triangles[i].p[1].y, mesh->triangles[i].p[1].z);
		//printf("p3 x: %f, y: %f z: %f\n\n", mesh->triangles[i].p[2].x, mesh->triangles[i].p[2].y, mesh->triangles[i].p[2].z);
		matrix_mult_3x1_4x4((mesh->triangles + i)->p + 0, mesh->rotation_mat_z, &rotated_z.p[0]);
		matrix_mult_3x1_4x4((mesh->triangles + i)->p + 1, mesh->rotation_mat_z, &rotated_z.p[1]);
		matrix_mult_3x1_4x4((mesh->triangles + i)->p + 2, mesh->rotation_mat_z, &rotated_z.p[2]);

		// printf("p1 x: %f, y: %f z: %f\n", rotated_z.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
		// printf("p2 x: %f, y: %f z: %f\n", rotated_z.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
		// printf("p3 x: %f, y: %f z: %f\n\n", rotated_z.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);

		matrix_mult_3x1_4x4(rotated_z.p + 0, mesh->rotation_mat_x, &rotated_xz.p[0]);
		matrix_mult_3x1_4x4(rotated_z.p + 1, mesh->rotation_mat_x, &rotated_xz.p[1]);
		matrix_mult_3x1_4x4(rotated_z.p + 2, mesh->rotation_mat_x, &rotated_xz.p[2]);

		matrix_mult_3x1_4x4(rotated_xz.p + 0, mesh->rotation_mat_x, &rotated_xyz.p[0]);
		matrix_mult_3x1_4x4(rotated_xz.p + 1, mesh->rotation_mat_x, &rotated_xyz.p[1]);
		matrix_mult_3x1_4x4(rotated_xz.p + 2, mesh->rotation_mat_x, &rotated_xyz.p[2]);
		// printf("p1 x: %f, y: %f z: %f\n", rotated_xz.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
		// printf("p2 x: %f, y: %f z: %f\n", rotated_xz.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
		// printf("p3 x: %f, y: %f z: %f\n\n", rotated_xz.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);
	
	


		//translated = mesh->triangles[i];
		translated = rotated_xyz;
		translated.p[0].z += 3.0f;
		translated.p[1].z += 3.0f;
		translated.p[2].z += 3.0f;


		translated.normal = cross_product(v3_sub(translated.p[1], translated.p[0]), v3_sub(translated.p[2], translated.p[0]));

		float normal_len = sqrtf(translated.normal.x * translated.normal.x + translated.normal.y * translated.normal.y + translated.normal.z * translated.normal.z);
		scale_vec3(&translated.normal, 1 / normal_len);
		if (dot_prod(translated.normal, v3_sub(translated.p[0], mesh->main->camera)) >= 0)
		//if (translated.normal.z > 0)
		{
			i++;
			continue ;
		}
		//translate_triangle_3d(&translated, traveled_dist);
		// add_vec3(translated.p + 0, &traveled_dist);
		// add_vec3(translated.p + 1, &traveled_dist);
		// add_vec3(translated.p + 2, &traveled_dist);
		//determine_centroid(&translated);
		//add_vec3(&mesh->center, &translated.centroid);


		matrix_mult_3x1_4x4(translated.p + 0, project_mat, &projected.p[0]);
		matrix_mult_3x1_4x4(translated.p + 1, project_mat, &projected.p[1]);
		matrix_mult_3x1_4x4(translated.p + 2, project_mat, &projected.p[2]);

		printf("p1 x: %f, y: %f z: %f\n", projected.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
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
		fill_triangle(mesh->img, &projected, (mesh->triangles + i)->col);
		
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

