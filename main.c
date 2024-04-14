/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/14 06:07:54 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>


void	translate_mesh_3d(t_mesh *mesh, t_vec3 v);

//#define ROT_X
//#define ROT_Y
//#define ROT_Z

bool	out_of_bound(int p[2])
{
	if (p[X] < 0 || p[X] >= WIDTH || p[Y] < 0 || p[Y] >= HEIGHT)
		return (true);
	return (false);
}

bool	print = true;

static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void	mod_cube_rotation(t_mesh *mesh, float delta_time)
{
	static float	theta = 0;

	theta += delta_time;

	#ifndef ROT_Z
	float rotation_mat_z[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#else
	// float rotation_mat_z[4][4] = {
	// 	{cosf(theta), sinf(theta), 0.0f, 0.0f},
	// 	{-sinf(theta), cosf(theta), 0.0f, 0.0f},
	// 	{0.0f, 0.0f, 1.0f, 0.0f},
	// 	{0.0f, 0.0f, 0.0f, 1.0f},
	// };
	float rotation_mat_z[4][4] = {
		{cosf(theta), -sinf(theta), 0.0f, 0.0f},
		{sinf(theta), cosf(theta), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};

	#endif
	ft_memcpy(mesh->rotation_mat_z, rotation_mat_z, sizeof(rotation_mat_z));
	#ifdef ROT_X
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f), 0.0f},
		{0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	#else
	float rotation_mat_x[4][4] = {
		{-1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, -1.0f}
	};
	#endif
	ft_memcpy(mesh->rotation_mat_x, rotation_mat_x, sizeof(rotation_mat_x));
	#ifndef ROT_Y
	float rotation_mat_y[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#else
	float rotation_mat_y[4][4]  = {
		{cosf(theta), 0.0f, sinf(theta), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sinf(theta), 0.0f, cosf(theta * 0.5f), 1.0f},
	 	{0.0f, 0.0f, 0.0f, 1.0f}
		// {1.0f, 0.0f, 0.0f, 0.0f},
		// {0.0f, 1.0f, 0.0f, 0.0f},
		// {0.0f, 0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f)},
		// {0.0f, 0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f)},
	};
	#endif
	ft_memcpy(mesh->rotation_mat_y, rotation_mat_y, sizeof(rotation_mat_y));
}

void	mod_cube_rotation2(t_mesh *mesh, float delta_time)
{
	static float	theta = 0;

	theta += delta_time;

	#ifndef ROT_Z

	float rotation_mat_z[4][4] = {
		{cosf(theta), sinf(theta), 0.0f, 0.0f},
		{-sinf(theta), cosf(theta), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	#else
	float rotation_mat_z[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#endif
	ft_memcpy(mesh->rotation_mat_z, rotation_mat_z, sizeof(rotation_mat_z));
	#ifdef ROT_X
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f), 0.0f},
		{0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	#else
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#endif
	ft_memcpy(mesh->rotation_mat_x, rotation_mat_x, sizeof(rotation_mat_x));
	#ifndef ROT_Y
	float rotation_mat_y[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#else
	float rotation_mat_y[4][4]  = {
		{cosf(theta), 0.0f, sinf(theta), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sinf(theta), 0.0f, cosf(theta * 0.5f), 1.0f},
	 	{0.0f, 0.0f, 0.0f, 1.0f}
		// {1.0f, 0.0f, 0.0f, 0.0f},
		// {0.0f, 1.0f, 0.0f, 0.0f},
		// {0.0f, 0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f)},
		// {0.0f, 0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f)},
	};
	#endif
	ft_memcpy(mesh->rotation_mat_y, rotation_mat_y, sizeof(rotation_mat_y));
}

// Print the window width and height.
static void ft_hook(void* param)
{
	t_main		*main_data = param;
	static int pixel = 0;
	static int color = RED;
	int	y = pixel / WIDTH;
	int x = pixel - (y * WIDTH);

	mod_cube_rotation(&main_data->cube, main_data->mlx->delta_time);
	draw_cube(&main_data->cube);
	mod_cube_rotation(&main_data->tetra, main_data->mlx->delta_time);
	draw_cube(&main_data->tetra);
	mod_cube_rotation2(&main_data->cube2, main_data->mlx->delta_time);
	static bool first = true;
	// t_triangle shift = {{{-0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f}}, 1};
	t_vec3 shift = {-0.5f,-0.5f,-0.5f};
	if (first) {
		//translate_mesh_3d(&main_data->cube, shift);
		translate_mesh_3d(&main_data->cube2, shift);
	}
	draw_cube(&main_data->cube2);
	first = false;
	// mlx_put_pixel(img, x, y, color);
	// pixel++;
	// if (pixel >= WIDTH * HEIGHT)
	// {
	// 	printf("filled !\n");
	// 	usleep(30000000);
	// 	pixel = 0;
	// 	color = ~color;
	// 	color |= 0xFF;
	// }
}

/*
for
	{x1,x2,x3,x4}
	*
	{x11,x12,x13,x14
	x21,x22,x23,x24
	x31,x32,x33,x34
	x41,x42,x43,x44}
*/
void	matrix_mult_3x1_4x4(t_vec3 *m_a, const float m_b[4][4], t_vec3 *re)
{
	float	w;

	re->p[X] = m_a->p[X] *  m_b[0][0] + m_a->p[Y] * m_b[1][0] + m_a->p[Z] * m_b[2][0] + m_b[3][0];
	re->p[Y] = m_a->p[X] *  m_b[0][1] + m_a->p[Y] * m_b[1][1] + m_a->p[Z] * m_b[2][1] + m_b[3][1];
	re->p[Z] = m_a->p[X] *  m_b[0][2] + m_a->p[Y] * m_b[1][2] + m_a->p[Z] * m_b[2][2] + m_b[3][2];
	w = m_a->p[X] *  m_b[0][3]+ m_a->p[Y] * m_b[1][3] + m_a->p[Z] * m_b[2][3] + m_b[3][3];
	if (w != 0.0f)
	{
		re->p[X] /= w;
		re->p[Y] /= w;
		re->p[Z] /= w;
	}
}

// void	()
// {
// 	static const float	project_mat[4][4] = PROJECTION_MATRIX;
	
// }

bool	init(void)
{
	get_fps_digit_texture();
	return (true);
}



void	ft_put_pixel(uint8_t *pixel_buffer, int x, int y, int color)
{
	//const int	index_mult = sizeof(int32_t);

	((uint32_t *) pixel_buffer)[x + y * WIDTH] = color;
}

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

void	translate_triangle_3d(t_triangle *tri_a, t_vec3 v)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
			tri_a->p[i].p[j] += v.p[j];
	}
}

void	translate_mesh_3d(t_mesh *mesh, t_vec3 v)
{
	for (int i = 0; i < mesh->count; i++)
	{
		translate_triangle_3d(mesh->triangles + i, v);
	}
}


void	determine_centroid(t_triangle *tri)
{
	t_vec3 s = {0,0,0};
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			s.p[j] += tri->p[i].p[j];
		}
	}
	for (int i = 0; i < 3; i++) {
		tri->centroid.p[i] = s.p[i] / 3.0;
	}
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
int32_t	main(void)
{
	mlx_image_t	*ob;
	t_main		m_data;

	if (!init())
		return (1);
	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(0, true);
	m_data.mlx = mlx_init(WIDTH, HEIGHT, "test", true);
	if (!m_data.mlx)
		ft_error();

	mlx_image_t* cube_img = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	if (!cube_img || (mlx_image_to_window(m_data.mlx, cube_img, 0, 0) < 0))
		ft_error();
	mlx_image_t* cube_img2 = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	if (!cube_img2 || (mlx_image_to_window(m_data.mlx, cube_img2, 0, 0) < 0))
		ft_error();
	mlx_image_t* tetra_img = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	if (!tetra_img || (mlx_image_to_window(m_data.mlx, tetra_img, 0, 0) < 0))
		ft_error();
	m_data.cube.img = cube_img;
	m_data.tetra.img = tetra_img;
	m_data.cube2.img = cube_img2;
	m_data.cube.d_time = &m_data.mlx->delta_time;
	m_data.cube2.d_time = &m_data.mlx->delta_time;
	m_data.tetra.d_time = &m_data.mlx->delta_time;
	fill_cube_mesh(&m_data.cube);
	fill_tetra_mesh(&m_data.tetra);
	fill_cube_mesh2(&m_data.cube2);
	//draw_cube(&m_data.cube);
	
	mlx_set_instance_depth(cube_img->instances, 2);
	mlx_set_instance_depth(cube_img2->instances, 1);
	mlx_set_instance_depth(tetra_img->instances, 3);
	first_ob_ball(m_data.mlx);
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_loop_hook(m_data.mlx, ft_hook, &m_data);
	
	int	pos_a[2] = {200, 200};
	int	pos_b[2] = {100, 100};
	mlx_loop_hook(m_data.mlx, display_fps_hook, m_data.mlx);
	mlx_loop(m_data.mlx);
	mlx_terminate(m_data.mlx);
	return (EXIT_SUCCESS);
}
