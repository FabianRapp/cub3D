/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/26 10:03:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

void	reset_pixel_buffer(uint8_t *pixels, float *depth)
{
	uint32_t	*buffer = (uint32_t *)pixels;

	for (int i = 0; i < WIDTH * HEIGHT; i++)
	{
		buffer[i] = BLACK;
		depth[i] = Z_FAR;
	}
	
}

void	handle_movement_per_frame(t_main *main_data)
{
	t_controls			controls;
	t_vec3				movement = {0, 0, 0};
	// t_vec3				x_z_unit_look_direct;

	// x_z_unit_look_direct = main_data->look_direct;
	// float	len_x_z_look = x_z_unit_look_direct.x + x_z_unit_look_direct.z;
	// x_z_unit_look_direct.x = (x_z_unit_look_direct.x * 2) / len_x_z_look;
	// x_z_unit_look_direct.z = (x_z_unit_look_direct.z * 2) / len_x_z_look;

	controls = main_data->controls;
	// if (state->up)
	// {
	// 	main_data->look_direct.y += 0.1;
	// }
	// // else if (state->left)
	// // {
	// // }
	// // else if (state->right)
	// // {
	// // }
	// else if (state->down)
	// {
	// 	main_data->look_direct.y -= 0.1;
	// }
	if (controls.state.up)
	{
		movement.x += main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_straight;
		movement.y += main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_straight;
		movement.z += main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_straight;
	}
	if (controls.state.left)
	{
		main_data->camera.x -= main_data->mlx->delta_time * controls.movement_speed_left;
		print_vec3(main_data->camera, "new camera: ");
		// movement.x += main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_left;
		// movement.y += main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_left;
		// movement.z += main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_left;
	}
	if (controls.state.right)
	{
		main_data->camera.x += main_data->mlx->delta_time * controls.movement_speed_right;
		print_vec3(main_data->camera, "new camera: ");
		// movement.x += main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_straight;
		// movement.y += main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_straight;
		// movement.z += main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_straight;
	}
	if (controls.state.back)
	{
		movement.x -= main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_back;
		movement.y -= main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_back;
		movement.z -= main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_back;
	}
	//multiply_vec3(&movement, &controls.movement_speed);
	if (controls.state.jump)
	{
		printf("jump\n");
		movement.y += controls.jump_height * main_data->mlx->delta_time;
	}
	if (controls.state.negative_jump)
	{
		printf("negative jump\n");
		movement.y -= controls.jump_height * main_data->mlx->delta_time;
	}
	if (!zero_f(movement.x) || !zero_f(movement.y) || !zero_f(movement.z))
	{
		add_vec3(&main_data->camera, &movement);
		print_vec3(main_data->camera, "new camera: ");
	}
	//
	//unit_vec3(&main_data->camera);
}

// Print the window width and height.
void ft_hook(void* param)
{
	t_main		*main_data = param;
	static int pixel = 0;
	static int color = RED;
	int	y = pixel / WIDTH;
	int x = pixel - (y * WIDTH);

	handle_movement_per_frame(main_data);
	ident_mat_4x4(main_data->world_mat);
	// rot_matz_4x4()
	// rot_matx_4x4()
	// rot_maty_4x4()
	// mat4x4_mult_mat4x4( , ,main_data->world_mat);
	reset_pixel_buffer(main_data->img->pixels, main_data->depth);
	//draw_skybox(&main_data->skybox);
	//ft_bzero(main_data->depth, sizeof(float) * WIDTH * HEIGHT);
	//mod_cube_rotation(&main_data->cube, main_data->mlx->delta_time);
	mod_cube_rotation(&main_data->custom, main_data->mlx->delta_time);
	//draw_mesh(&main_data->cube);
	draw_mesh(&main_data->axis);
	//draw_mesh(&main_data->custom);
	//mod_cube_rotation(&main_data->tetra, main_data->mlx->delta_time);
	//draw_mesh(&main_data->tetra);
//	mod_cube_rotation2(&main_data->cube2, main_data->mlx->delta_time);
	for (int i = 0; i < main_data->nb; i++)
	{
		const float rotation_mat[4][4] = {
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f,1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f},
		};
		ft_memcpy(main_data->objs[i].rotation_mat_x, rotation_mat, sizeof(rotation_mat));
		ft_memcpy(main_data->objs[i].rotation_mat_y, rotation_mat, sizeof(rotation_mat));
		ft_memcpy(main_data->objs[i].rotation_mat_z, rotation_mat, sizeof(rotation_mat));
		//mod_cube_rotation(&main_data->objs[0], main_data->mlx->delta_time);
		draw_mesh(&main_data->objs[i]);
	}
	//draw_mesh(&main_data->cube2);
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

void	scale_triangle_3d(t_triangle *tri_a, float scalar)
{
	for (int i = 0; i < 3; i++)
	{
		
		scale_vec3(tri_a->p + i, scalar);
	}
}

void	translate_mesh_3d(t_mesh *mesh, t_vec3 v)
{
	for (int i = 0; i < mesh->count; i++)
	{
		translate_triangle_3d(mesh->triangles + i, v);
	}
}

void	scale_mesh_3d(t_mesh *mesh, float scalar)
{
	for (int i = 0; i < mesh->count; i++)
	{
		scale_triangle_3d(mesh->triangles + i, scalar);
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
			//print_vec3(j[vertex], "[0]");
		}
		// mesh_arr[i].triangles = (t_triangle *) malloc(sizeof(t_triangle) * 24);
		t_triangle trians[4] = {
			{{vertex[0], vertex[1], vertex[2]}, MAGENTA, 0},
			{{vertex[0], vertex[1], vertex[3]}, MAGENTA, 0},
			{{vertex[1], vertex[2], vertex[3]}, MAGENTA, 0},
			{{vertex[2], vertex[3], vertex[0]}, MAGENTA, 0},
		};
		// for (int j = 0; j < 4; j++) {
		// 	print_vec3(trians->p[j], "0");
		// }
		// print_vec3(trians[0].p[0], "tr[0]");
		// print_vec3(trians[1].p[0], "tr[1]");
		// print_vec3(trians[2].p[0], "tr[2]");
		// print_vec3(trians[3].p[0], "tr[3]");
		//printf("\n");
		m_data->objs[i].triangles = ft_memdup(&trians, sizeof(trians));
		m_data->objs[i].img = mlx_new_image(m_data->mlx, WIDTH, HEIGHT);
		if (!m_data->objs[i].img || (mlx_image_to_window(m_data->mlx, m_data->objs[i].img, 0, 0) < 0))
			ft_error();
		mlx_set_instance_depth(m_data->objs[i].img->instances, 2*nb - i);
		m_data->objs[i].momentum = v3_zero();
		m_data->objs[i].momentum.x = generate_random_float();
		m_data->objs[i].momentum.y = generate_random_float();
		m_data->objs[i].count = 4;
		m_data->objs[i].d_time = &m_data->mlx->delta_time;
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

// pauses the loop while key is held
void	pause_key_hook(void *param)
{
	t_main	*data;

	usleep(100000);
	//data->mlx->delta_time = d_time;
}

void	ft_key_hook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == MLX_KEY_SPACE)
		pause_key_hook(param);
}


void	init_key_hooks(t_main *main_data)
{
	mlx_key_hook(main_data->mlx, &ft_key_hook, &main_data);
}

void	arrow_key_handler(mlx_key_data_t keydata, void *param)
{
	t_controls	*controls;

	controls = &((t_main *)param)->controls;
	if (keydata.action == MLX_REPEAT)
	{
		if (keydata.key == MLX_KEY_UP)
			controls->state.up = true;
		if (keydata.key == MLX_KEY_LEFT)
			controls->state.left = true;
		if (keydata.key == MLX_KEY_RIGHT)
			controls->state.right = true;
		if (keydata.key == MLX_KEY_DOWN)
			controls->state.back = true;
	}
	else if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_UP)
			controls->state.up = false;
		if (keydata.key == MLX_KEY_LEFT)
			controls->state.left = false;
		if (keydata.key == MLX_KEY_RIGHT)
			controls->state.right = false;
		if (keydata.key == MLX_KEY_DOWN)
			controls->state.back = false;
	}
}

void	jump_key_handler(mlx_key_data_t keydata, void *param)
{
	t_controls	*controls;

	controls = &((t_main *)param)->controls;
	if (keydata.key == MLX_KEY_SPACE && keydata.action != MLX_RELEASE)
	{
		controls->state.jump = true;
	}
	else
	{
		controls->state.jump = false;
	}
	if (keydata.key == MLX_KEY_Z && keydata.action != MLX_RELEASE)
		controls->state.negative_jump = true;
	else
		controls->state.negative_jump = false;
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key >= MLX_KEY_RIGHT && keydata.key <= MLX_KEY_UP)
		arrow_key_handler(keydata, param);
	if (keydata.key == MLX_KEY_SPACE || keydata.key == MLX_KEY_Z)
		jump_key_handler(keydata, param);
}


int32_t	main(void)
{
	mlx_image_t	*ob;
	t_main		m_data;
	const t_vec3	init_cam = {.x = 0, .y = 0, .z = 0};
	const t_vec3	init_look_direct = {.x = 0, .y = 0, .z = 1};


	ft_bzero(&m_data, sizeof(m_data));
	m_data.controls.jump_height = 1;
	m_data.controls.movement_speed_straight = 1;
	m_data.controls.movement_speed_left = 1;
	m_data.controls.movement_speed_right = 1;
	m_data.controls.movement_speed_back = 1;
	ft_memcpy(&m_data.camera, &init_cam, sizeof(init_cam));
	ft_memcpy(&m_data.look_direct, &init_look_direct, sizeof(init_look_direct));
	srand(time(NULL));
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
	mlx_image_t* cube_img2 = cube_img;
	mlx_image_t* tetra_img = cube_img;
	m_data.img = cube_img;
	// mlx_image_t* cube_img2 = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	// if (!cube_img2 || (mlx_image_to_window(m_data.mlx, cube_img2, 0, 0) < 0))
	// 	ft_error();
	// mlx_image_t* tetra_img = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	// if (!tetra_img || (mlx_image_to_window(m_data.mlx, tetra_img, 0, 0) < 0))
	// 	ft_error();
	m_data.cube.img = cube_img;
	m_data.tetra.img = tetra_img;
	m_data.cube2.img = cube_img2;
	m_data.cube.d_time = &m_data.mlx->delta_time;
	m_data.cube2.d_time = &m_data.mlx->delta_time;
	m_data.tetra.d_time = &m_data.mlx->delta_time;
	
	load_obj_file("./", "axis.obj", &m_data.axis, &m_data);
	
	load_obj_file("RAN Easter Egg 2024 - OBJ/", "RAN Easter Egg 2024 - OBJ/RAN_Easter_Egg_2024_Low_Poly.obj", &m_data.custom, &m_data);
	//load_obj_file("lego_obj/", "lego_obj/lego obj.obj", &m_data.custom, &m_data);
	//load_obj_file("RAN Easter Egg 2024 - OBJ/", "RAN Easter Egg 2024 - OBJ/RAN_Easter_Egg_2024_High_Poly.obj", &m_data.custom, &m_data);
	//load_obj_file("objs/", "objs/HorseArmor.obj", &m_data.custom, &m_data);
	//load_obj_file("teapot/", "teapot/teapot.obj", &m_data.custom, &m_data);
	// load_obj_file("obj/", "obj/crates.obj", &m_data.custom, &m_data);
	//load_obj_file("22-trees_9_obj/", "22-trees_9_obj/trees9.obj", &m_data.custom, &m_data);
	//fill_cube_mesh(&m_data.cube, &m_data);
	fill_skybox_mesh(&m_data.skybox, &m_data);
	//fill_tetra_mesh(&m_data.tetra, &m_data);
	fill_cube_mesh2(&m_data.cube2, &m_data);
	//draw_mesh(&m_data.cube);
	draw_objects(0, &m_data);
	mlx_set_instance_depth(cube_img->instances, 2);
	mlx_set_instance_depth(cube_img2->instances, 1);
	mlx_set_instance_depth(tetra_img->instances, 3);
	//first_ob_ball(m_data.mlx);
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	init_key_hooks(&m_data);
	mlx_loop_hook(m_data.mlx, ft_hook, &m_data);
	int	pos_a[2] = {200, 200};
	int	pos_b[2] = {100, 100};
	mlx_key_hook(m_data.mlx, &key_hook, &m_data);
	mlx_loop_hook(m_data.mlx, display_fps_hook, m_data.mlx);
	mlx_loop(m_data.mlx);
	mlx_terminate(m_data.mlx);
	return (EXIT_SUCCESS);
}
