
#include <cub3D.h>
#include <MLX42.h>

void	draw_line(mlx_image_t *image, int start_pos[2], int target_pixel[2], int color)
{
	int	pos[2];
	int	dist_x;
	int	dist_y;
	int	direct_x;
	int	direct_y;
	int	last_error;
	int	cur_error;

	pos[X] = start_pos[X];
	pos[Y] = start_pos[Y];
	direct_x = (pos[X] > target_pixel[X]) * -1;
	direct_y = (pos[Y] > target_pixel[Y]) * -1;
	if (!direct_x)
		direct_x++;
	if (!direct_y)
		direct_y++;
	// handle vert/hor lines extra
	dist_x = abs(target_pixel[X] - pos[X]);
	dist_y = abs(target_pixel[Y] - pos[Y]);
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
	// 	printf("target x: %d target y: %d\n", target_pixel[X], target_pixel[Y]);
	// 	printf("pos x: %d pos y: %d\n", pos[X], pos[Y]);
	// 	//printf("red dist: %d\n", dist_x + dist_y);
	// }
	if (pos[X] == target_pixel[X] || pos[Y] == target_pixel[Y])
	{
		if (pos[X] == target_pixel[X])
		{
			while (pos[Y] != target_pixel[Y])
			{
				if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
					ft_put_pixel(image->pixels, pos[X], pos[Y], color);
					//mlx_put_pixel(image, pos[X], pos[Y], color);
				pos[Y] += direct_y;
			}
		}
		if (pos[Y] == target_pixel[Y])
		{
			while (pos[X] != target_pixel[X])
			{
				if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
					ft_put_pixel(image->pixels, pos[X], pos[Y], color);
					//mlx_put_pixel(image, pos[X], pos[Y], color);
				else
					printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
				pos[X] += direct_x;
			}
		}
		if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
			ft_put_pixel(image->pixels, pos[X], pos[Y], color);
			//mlx_put_pixel(image, pos[X], pos[Y], color);
		else
			printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
		return ;
	}

	last_error = dist_x - dist_y;
	while (pos[X] != target_pixel[X] || pos[Y] != target_pixel[Y])
	{
		if (pos[X] >= 0 && pos[X] < WIDTH && pos[Y] >= 0 && pos[Y] < HEIGHT)
			ft_put_pixel(image->pixels, pos[X], pos[Y], color);
			//mlx_put_pixel(image, pos[X], pos[Y], color);
		else
			printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
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
	else
		printf("error: xpos: %d ypos: %d\n", pos[X], pos[Y]);
}

void	draw_triangle(mlx_image_t *img, int p1[2], int p2[2], int p3[2], uint32_t color)
{
	draw_line(img, p2, p3, color);
	draw_line(img, p3, p1, color);
	draw_line(img, p1, p2, color);
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
	// static double	theta = 0;
	// 	theta += *mesh->d_time;
	t_vec3	traveled_dist;
	traveled_dist.p[X] = mesh->momentum.p[X] * *mesh->d_time;
	traveled_dist.p[Y] = mesh->momentum.p[Y] * *mesh->d_time;
	traveled_dist.p[Z] = mesh->momentum.p[Z] * *mesh->d_time;
	translate_mesh_3d(mesh, traveled_dist);
	bool done = false;
	mesh->center.p[X] = 0;
	mesh->center.p[Y] = 0;
	mesh->center.p[Z] = 0;
	while (i < mesh->count)
	{
		// printf("p1 x: %f, y: %f z: %f\n", mesh->triangles[i].p[0].p[X], mesh->triangles[i].p[0].p[Y], mesh->triangles[i].p[0].p[Z]);
		// printf("p2 x: %f, y: %f z: %f\n", mesh->triangles[i].p[1].p[X], mesh->triangles[i].p[1].p[Y], mesh->triangles[i].p[1].p[Z]);
		// printf("p3 x: %f, y: %f z: %f\n\n", mesh->triangles[i].p[2].p[X], mesh->triangles[i].p[2].p[Y], mesh->triangles[i].p[2].p[Z]);
		matrix_mult_3x1_4x4((mesh->triangles + i)->p + 0, mesh->rotation_mat_z, &rotated_z.p[0]);
		matrix_mult_3x1_4x4((mesh->triangles + i)->p + 1, mesh->rotation_mat_z, &rotated_z.p[1]);
		matrix_mult_3x1_4x4((mesh->triangles + i)->p + 2, mesh->rotation_mat_z, &rotated_z.p[2]);

		// printf("p1 x: %f, y: %f z: %f\n", rotated_z.p[0].p[X], rotated_z.p[0].p[Y], rotated_z.p[0].p[Z]);
		// printf("p2 x: %f, y: %f z: %f\n", rotated_z.p[1].p[X], rotated_z.p[1].p[Y], rotated_z.p[1].p[Z]);
		// printf("p3 x: %f, y: %f z: %f\n\n", rotated_z.p[2].p[X], rotated_z.p[2].p[Y], rotated_z.p[2].p[Z]);

		matrix_mult_3x1_4x4(rotated_z.p + 0, mesh->rotation_mat_x, &rotated_xz.p[0]);
		matrix_mult_3x1_4x4(rotated_z.p + 1, mesh->rotation_mat_x, &rotated_xz.p[1]);
		matrix_mult_3x1_4x4(rotated_z.p + 2, mesh->rotation_mat_x, &rotated_xz.p[2]);

		matrix_mult_3x1_4x4(rotated_xz.p + 0, mesh->rotation_mat_x, &rotated_xyz.p[0]);
		matrix_mult_3x1_4x4(rotated_xz.p + 1, mesh->rotation_mat_x, &rotated_xyz.p[1]);
		matrix_mult_3x1_4x4(rotated_xz.p + 2, mesh->rotation_mat_x, &rotated_xyz.p[2]);
		// printf("p1 x: %f, y: %f z: %f\n", rotated_xz.p[0].p[X], rotated_z.p[0].p[Y], rotated_z.p[0].p[Z]);
		// printf("p2 x: %f, y: %f z: %f\n", rotated_xz.p[1].p[X], rotated_z.p[1].p[Y], rotated_z.p[1].p[Z]);
		// printf("p3 x: %f, y: %f z: %f\n\n", rotated_xz.p[2].p[X], rotated_z.p[2].p[Y], rotated_z.p[2].p[Z]);
	
		//translated = mesh->triangles[i];
		translated = rotated_xz;
		translated.p[0].p[Z] += 3.0f;
		translated.p[1].p[Z] += 3.0f;
		translated.p[2].p[Z] += 3.0f;

		determine_centroid(&translated);
		add_vec3(&mesh->center, &translated.centroid);

		matrix_mult_3x1_4x4(translated.p + 0, project_mat, &projected.p[0]);
		matrix_mult_3x1_4x4(translated.p + 1, project_mat, &projected.p[1]);
		matrix_mult_3x1_4x4(translated.p + 2, project_mat, &projected.p[2]);

		// printf("p1 x: %f, y: %f z: %f\n", projected.p[0].p[X], rotated_z.p[0].p[Y], rotated_z.p[0].p[Z]);
		// printf("p2 x: %f, y: %f z: %f\n", projected.p[1].p[X], rotated_z.p[1].p[Y], rotated_z.p[1].p[Z]);
		// printf("p3 x: %f, y: %f z: %f\n\n", projected.p[2].p[X], rotated_z.p[2].p[Y], rotated_z.p[2].p[Z]);

		// scale the points
		projected.p[0].p[X] += 1.0f;
		projected.p[0].p[Y] += 1.0f;
		projected.p[1].p[X] += 1.0f;
		projected.p[1].p[Y] += 1.0f;
		projected.p[2].p[X] += 1.0f;
		projected.p[2].p[Y] += 1.0f;

		projected.p[0].p[X] *= 0.5f * (float)WIDTH;
		projected.p[0].p[Y] *= 0.5f * (float)HEIGHT;
		projected.p[1].p[X] *= 0.5f * (float)WIDTH;
		projected.p[1].p[Y] *= 0.5f * (float)HEIGHT;
		projected.p[2].p[X] *= 0.5f * (float)WIDTH;
		projected.p[2].p[Y] *= 0.5f * (float)HEIGHT;

		p_2d[0][X] = (int)round(projected.p[0].p[X]);
		p_2d[0][Y] = (int)round(projected.p[0].p[Y]);
		p_2d[1][X] = (int)round(projected.p[1].p[X]);
		p_2d[1][Y] = (int)round(projected.p[1].p[Y]);
		p_2d[2][X] = (int)round(projected.p[2].p[X]);
		p_2d[2][Y] = (int)round(projected.p[2].p[Y]);
		if (!done && (out_of_bound) (p_2d[0]) || out_of_bound(p_2d[1]) || out_of_bound(p_2d[2]))
		{
			//mesh->
			done = true;
			mesh->momentum.p[X] *= -1;
			mesh->momentum.p[Y] *= -1;
			mesh->momentum.p[Z] *= -1;
		}
		draw_triangle(mesh->img, p_2d[0], p_2d[1], p_2d[2], (mesh->triangles + i)->col);
		i++;
	}
	mesh->center_pull = scale_vec3(&mesh->center, 1.0f / (float) mesh->count);
	// length_vec3(&mesh->center) / mesh->count;
}
