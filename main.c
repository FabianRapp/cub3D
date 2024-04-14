/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/14 06:31:52 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>



// Print the window width and height.
void ft_hook(void* param)
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


// void	()
// {
// 	static const float	project_mat[4][4] = PROJECTION_MATRIX;
	
// }

bool	init(void)
{
	get_fps_digit_texture();
	return (true);
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
