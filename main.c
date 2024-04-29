/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/29 20:33:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>



void	handle_movement_per_frame(t_main *main_data)
{
	t_controls			controls;
	t_vec3				movement = {0, 0, 0, .w = 1};

	controls = main_data->controls;
	if (controls.state.up)
	{
		movement.x += main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_straight;
		movement.y += main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_straight;
		movement.z += main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_straight;
	}
	if (controls.state.left)
	{
		t_vec3	no_y = {.x = 0.5, .y = 0, .z = 0.5, .w = 1};
		t_vec3	forward = v3_sub(main_data->look_direct, main_data->camera);
		unit_vec3(&forward);
		t_vec3 left = cross_product(main_data->up, forward);
		unit_vec3(&left);
		main_data->camera.x -= main_data->mlx->delta_time * controls.movement_speed_left * left.x;
		main_data->camera.z -= main_data->mlx->delta_time * controls.movement_speed_left * left.z;
		//print_vec3(main_data->camera, "new camera: ");
		// movement.x += main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_left;
		// movement.y += main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_left;
		// movement.z += main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_left;
	}
	if (controls.state.right)
	{
		t_vec3	no_y = {.x = 0.5, .y = 0, .z = 0.5, .w = 1};
		t_vec3	forward = v3_sub(main_data->look_direct, main_data->camera);
		unit_vec3(&forward);
		t_vec3 right = cross_product(main_data->up, forward);
		unit_vec3(&right);
		main_data->camera.x += main_data->mlx->delta_time * controls.movement_speed_right * right.x;
		main_data->camera.z += main_data->mlx->delta_time * controls.movement_speed_right * right.z;
		//print_vec3(main_data->camera, "new camera: ");
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
		movement.y -= controls.jump_height * main_data->mlx->delta_time;
	}
	if (controls.state.negative_jump)
	{
		movement.y += controls.jump_height * main_data->mlx->delta_time;
	}
	if (!zero_f(movement.x) || !zero_f(movement.y) || !zero_f(movement.z))
	{
		add_vec3(&main_data->camera, &movement);
	}
}



// Print the window width and height.
void	ft_hook(void* param)
{
	t_main		*main_data = param;
	static int pixel = 0;
	static int color = RED;
	int	y = pixel / WIDTH;
	int x = pixel - (y * WIDTH);

	if (menu_handler(main_data) == true)
		return ;
	if (main_data->settings.paused == true)
		return ;
	handle_movement_per_frame(main_data);
	ident_mat_4x4(main_data->world_mat);
	reset_pixel_buffer(main_data->img->pixels, main_data->depth);
	//draw_skybox(&main_data->skybox);
	//ft_bzero(main_data->depth, sizeof(float) * WIDTH * HEIGHT);
	//mod_cube_rotation(&main_data->cube, main_data->mlx->delta_time);
	mod_cube_rotation(&main_data->custom, main_data->mlx->delta_time);
	//draw_mesh(&main_data->cube);
	draw_mesh(&main_data->axis);
	draw_mesh(&main_data->custom);
	//mod_cube_rotation(&main_data->tetra, main_data->mlx->delta_time);
	//draw_mesh(&main_data->tetra);
//	mod_cube_rotation2(&main_data->cube2, main_data->mlx->delta_time);
	//draw_mesh(&main_data->cube2);
	// mlx_put_pixel(img, x, y, color);
	// pixel++;
	// if (pixel >= WIDTH * HEIGHT)
	// {
	// 	fprintf(stderr, "filled !\n");
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

void	cleanup_exit(void *m_data)
{
	t_main	*main_data;

	main_data = (t_main *)m_data;
	fprintf(stderr, "exiting..\n");
	exit(0);
}

void	init_cursor(t_main *main_data)
{
	mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_DISABLED);
	main_data->settings.cursor_lock = true;
	main_data->settings.cursor_hide = true;
	main_data->settings.mouse_sens = 0.5;
	//mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2); does not work before loop
	mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_HIDDEN);
}

void	init_settings(t_main *main_data)
{
	main_data->settings.paused = false;
	main_data->menu.state = MENU_CLOSED;
	init_cursor(main_data);
}

int32_t	main(void)
{
	mlx_image_t	*ob;
	t_main		m_data;
	const t_vec3	init_cam = {.x = 0, .y = 0, .z = 0, .w = 1};
	const t_vec3	init_look_direct = {.x = 0, .y = 0, .z = 1, .w = 1};
	const t_vec3	init_up = {.x = 0, .y = 1, .z = 0, .w = 1};

	int i = 0;
	// m_data.monitor_width = 0;
	// while (!m_data.monitor_width)
	// {
	// 	mlx_get_monitor_size(i++, &m_data.monitor_width, &m_data.monitor_height);
	// 	fprintf(stderr, "monitor width: %d monitor height: %d\n", m_data.monitor_width, m_data.monitor_height);
	// }
	ft_bzero(&m_data.menu, sizeof(m_data.menu));
	ft_bzero(&m_data, sizeof(m_data));
	m_data.controls.jump_height = 3;
	m_data.controls.movement_speed_straight = 3;
	m_data.controls.movement_speed_left = 3;
	m_data.controls.movement_speed_right = 3;
	m_data.controls.movement_speed_back = 3;
	m_data.pitch = 0;
	m_data.yaw = 0;
	m_data.roll = 0;

	ft_memcpy(&m_data.camera, &init_cam, sizeof(init_cam));
	ft_memcpy(&m_data.up, &init_up, sizeof(init_up));
	ft_memcpy(&m_data.look_direct, &init_look_direct, sizeof(init_look_direct));
	srand(time(NULL));
	if (!init())
		return (1);
	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(0, true);
	m_data.mlx = mlx_init(WIDTH, HEIGHT, "test", true);
	if (!m_data.mlx)
		ft_error(&m_data);
	
	mlx_image_t* cube_img = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	if (!cube_img || (mlx_image_to_window(m_data.mlx, cube_img, 0, 0) < 0))
		ft_error(&m_data);
	
	mlx_image_t* cube_img2 = cube_img;
	mlx_image_t* tetra_img = cube_img;
	m_data.img = cube_img;
	m_data.cube.img = cube_img;
	m_data.tetra.img = tetra_img;
	m_data.cube2.img = cube_img2;
	m_data.cube.d_time = &m_data.mlx->delta_time;
	m_data.cube2.d_time = &m_data.mlx->delta_time;
	m_data.tetra.d_time = &m_data.mlx->delta_time;

	load_obj_file("./", "axis.obj", &m_data.axis, &m_data);

	//load_obj_file("RAN Easter Egg 2024 - OBJ/", "RAN Easter Egg 2024 - OBJ/RAN_Easter_Egg_2024_Low_Poly.obj", &m_data.custom, &m_data);
	//load_obj_file("lego_obj/", "lego_obj/lego obj.obj", &m_data.custom, &m_data);
	//load_obj_file("RAN Easter Egg 2024 - OBJ/", "RAN Easter Egg 2024 - OBJ/RAN_Easter_Egg_2024_High_Poly.obj", &m_data.custom, &m_data);
	load_obj_file("objs/", "objs/HorseArmor.obj", &m_data.custom, &m_data);
	//load_obj_file("teapot/", "teapot/teapot.obj", &m_data.custom, &m_data);
	// load_obj_file("obj/", "obj/crates.obj", &m_data.custom, &m_data);
	//load_obj_file("22-trees_9_obj/", "22-trees_9_obj/trees9.obj", &m_data.custom, &m_data);
	//fill_cube_mesh(&m_data.cube, &m_data);
	fill_skybox_mesh(&m_data.skybox, &m_data);
	//fill_tetra_mesh(&m_data.tetra, &m_data);
	fill_cube_mesh2(&m_data.cube2, &m_data);
	//draw_mesh(&m_data.cube);
	mlx_set_instance_depth(m_data.img->instances, MAIN_RENDER_DEPTH);
	//first_ob_ball(m_data.mlx);
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	// int	pos_a[2] = {200, 200};
	// int	pos_b[2] = {100, 100};
	mlx_mouse_hook(m_data.mlx, mouse_hook, &m_data); // first
	mlx_key_hook(m_data.mlx, &key_hook, &m_data); // second
	mlx_cursor_hook(m_data.mlx, cursor_hook, &m_data); // third
	mlx_loop_hook(m_data.mlx, display_fps_hook, m_data.mlx); // dosnt matter when
	mlx_loop_hook(m_data.mlx, ft_hook, &m_data); // last
	mlx_close_hook(m_data.mlx, cleanup_exit, &m_data); // dosnt matter when
	init_settings(&m_data);
	mlx_loop(m_data.mlx);
	mlx_terminate(m_data.mlx);
	return (EXIT_SUCCESS);
}
