/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/16 14:13:54 by frapp            ###   ########.fr       */
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
	for (int i = 0; i < main_data->nb; i++)
	{
		mod_cube_rotation(&main_data->objs[0], main_data->mlx->delta_time);
		draw_cube(&main_data->objs[0]);
	}
	static bool first = true;
	// t_triangle shift = {{{-0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f}, {-0.5f,-0.5f,-0.5f}}, 1};
	t_vec3 shift = {-0.5f,-0.5f,-0.5f};
	if (first) {
		translate_mesh_3d(&main_data->cube, shift);
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
		add_vec3(tri_a->p + i, &v);
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
		add_vec3(&s, tri->p + i);
	}
	tri->centroid.x = s.x / 3.0;
	tri->centroid.y = s.y / 3.0;
	tri->centroid.z = s.z / 3.0;
}
void	draw_objects(int nb, t_main *m_data)
{
	const float max_edge_size = 0.1;
	const float min_edge_szie = 0.01;
	t_vec3	tetrahedron[4] = {
		{1.71, 1.41, 2.22},  // Vertex A
		{1.71, 2.22, 1.00},  // Vertex B
		{2.41, 1.00, 1.00},  // Vertex C
		{1.00, 1.00, 1.00}   // Vertex D
	};
	m_data->objs =  malloc(sizeof(t_mesh) * nb);
	m_data->nb = nb;
	// t_vec3 points[4]
	// int indexes[24][3] = {{1,3,2}, {1,2,4}, {1,4,2}, {1,3,4}, {1,4,3}, {2,1,3}, {2,3,1}, {2,1,4}, {2,4,1}, {2,3,4}, {2,4,3}, {3,1,2}, {3,2,1}, {3,1,4}, {3,4,1}, {3,2,4}, {3,4,2}, {4,1,2}, {4,2,1}, {4,1,3}, {4,3,1}, {4,2,3}, {4,3,2}};
	
	for (int i = 0; i < nb; i++)
	{
		srand(i);
		t_vec3	v = v3_random();
		// mesh_arr[i].triangles = (t_triangle *) malloc(sizeof(t_triangle) * 4);
		// mesh_arr[i].triangles->p[0] = v3_random();
		// t_triangle tmp = {
		// 	.p = {{v.x * 1.71, v.y * 1.41, v.z * 2.22},  // Vertex A
		// 	 {v.x * 1.71, v.y * 2.22, v.z * 1.00},  // Vertex B
		// 	 {v.x * 2.41, v.y * 1.00, v.z * 1.00},  // Vertex C
		// 	 {{v.x * 1.00, v.y * 1.00, v.z * 1.00}}   // Vertex D
		// };
		//mesh_arr[i].triangles;
		t_vec3	vertex[4] = {
			{v.x * 1.71f, v.y * 1.41f, v.z * 2.22f},// Vertex A
			{v.x * 1.71f, v.y * 2.22f, v.z * 1.00f},// Vertex B
			{v.x * 2.41f, v.y * 1.00f, v.z * 1.00f},// Vertex C
			{v.x * 1.00f, v.y * 1.00f, v.z * 1.00f}
		};// Vertex D
		for (int j = 0; j < 4; j++) {
			print_vec3(j[vertex], "[0]");
		}
		// mesh_arr[i].triangles = (t_triangle *) malloc(sizeof(t_triangle) * 24);
		t_triangle trians[4] = {
			{{vertex[0], vertex[1], vertex[2]}, COL2, 0},
			{{vertex[0], vertex[1], vertex[3]}, COL2, 0},
			{{vertex[1], vertex[2], vertex[3]}, COL2, 0},
			{{vertex[2], vertex[3], vertex[0]}, COL2, 0},
		};
		// for (int j = 0; j < 4; j++) {
		// 	print_vec3(trians->p[j], "0");
		// }
		// print_vec3(trians[0].p[0], "tr[0]");
		// print_vec3(trians[1].p[0], "tr[1]");
		// print_vec3(trians[2].p[0], "tr[2]");
		// print_vec3(trians[3].p[0], "tr[3]");
		printf("\n");
		m_data->objs [i].triangles = ft_memdup(&trians, sizeof(trians));
		m_data->objs [i].img = mlx_new_image(m_data->mlx, WIDTH, HEIGHT);
		if (!m_data->objs [i].img || (mlx_image_to_window(m_data->mlx, m_data->objs [i].img, 0, 0) < 0))
			ft_error();
		mlx_set_instance_depth(m_data->objs [i].img->instances, 2*nb - i);
		m_data->objs [i].momentum = v3_zero();
		m_data->objs [i].count = 4;
		m_data->objs [i].d_time = &m_data->mlx->delta_time;
		// for (int j = 0; j < 24; j++) {
		// 	for (int k = 0; k < 3; k++) {
		// 		t_vec3 tmp = vertex[indexes[j][k]];
		// 		for (int L = 0; L < 3; L++) {
		// 			mesh_arr[i].triangles->p[k].p[L] = tmp[L];
		// 		}
		// 	}
		// }
	}
}
#include <sys/time.h>

int32_t	main(void)
{
	mlx_image_t	*ob;
	t_main		m_data;
	struct timeval			asd;

	gettimeofday(&asd, NULL);
srand(asd.tv_usec);
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
	draw_objects(0, &m_data);
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
