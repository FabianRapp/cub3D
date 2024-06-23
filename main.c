/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/06/18 00:55:44 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "includes/menu.h"

void	cursor_movement_per_frame(t_main *main_data)
{
	main_data->world_data.look_direct = v3_add(main_data->world_data.look_direct, get_direction(main_data->pitch, main_data->yaw, main_data->roll));
	unit_vec3(&main_data->world_data.look_direct);
	if (main_data->settings.cursor_lock)
	{
		mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);
	}
}

void	handle_movement_per_frame(t_main *main_data)
{
	t_controls			controls;
	t_vec3				movement = {0, 0, 0, .w = 1};

	cursor_movement_per_frame(main_data);
	controls = main_data->controls;
	if (controls.state.up)
	{
		movement.x += main_data->world_data.look_direct.x * main_data->mlx->delta_time * controls.movement_speed_straight;
		movement.y += main_data->world_data.look_direct.y * main_data->mlx->delta_time * controls.movement_speed_straight;
		movement.z += main_data->world_data.look_direct.z * main_data->mlx->delta_time * controls.movement_speed_straight;
	}
	if (controls.state.left)
	{
		t_vec3 left = cross_product(main_data->world_data.look_direct, main_data->world_data.up);
		unit_vec3(&left);
		main_data->world_data.camera.x -= main_data->mlx->delta_time * controls.movement_speed_left * left.x;
		main_data->world_data.camera.z -= main_data->mlx->delta_time * controls.movement_speed_left * left.z;
	}
	if (controls.state.right)
	{
		t_vec3 right = cross_product(main_data->world_data.up, main_data->world_data.look_direct);
		unit_vec3(&right);
		main_data->world_data.camera.x -= main_data->mlx->delta_time * controls.movement_speed_right * right.x;
		main_data->world_data.camera.z -= main_data->mlx->delta_time * controls.movement_speed_right * right.z;
	}
	if (controls.state.back)
	{
		movement.x -= main_data->world_data.look_direct.x * main_data->mlx->delta_time * controls.movement_speed_back;
		movement.y -= main_data->world_data.look_direct.y * main_data->mlx->delta_time * controls.movement_speed_back;
		movement.z -= main_data->world_data.look_direct.z * main_data->mlx->delta_time * controls.movement_speed_back;
	}
	if (controls.state.jump)
		movement.y += controls.jump_height * main_data->mlx->delta_time;
	if (controls.state.negative_jump)
		movement.y -= controls.jump_height * main_data->mlx->delta_time;
	if (!zero_f(movement.x) || !zero_f(movement.y) || !zero_f(movement.z))
		add_vec3(&main_data->world_data.camera, &movement);
}

static inline void	reset_depth(double *depth)
{
	static const double	z_far = (double)Z_FAR;
	//static const double		z_far = -(double)Z_FAR;
	static const int32_t	pixel_count = WIDTH * HEIGHT;

	asm volatile(
		//"vmovddup %[val], %%ymm0\n\t"
		"vbroadcastsd %[val], %%ymm1\n\t"
		"mov %[cnt], %%eax\n\t"
		"z_buffer_reset_loop:\n\t"
		"vmovapd %%ymm1, 0x00(%[dep])\n\t"
		"vmovapd %%ymm1, 0x20(%[dep])\n\t"
		"add $64, %[dep]\n\t"
		"sub $8, %%eax\n\t"
		//"vprefetch1  \n\t"
		"jnz z_buffer_reset_loop\n\t"
		: [dep] "+r" (depth)
		: [val] "m" (z_far), [cnt] "r" (pixel_count)
		: "eax", "ymm0", "memory"
	);
}

//% of line by line profile
static inline void	reset_pixel_buffer_main(uint8_t *pixels, double *depth)
{
	uint64_t	*buffer = (uint64_t *)pixels;
	const uint64_t	black = (((uint64_t)BLACK<<32) | BLACK);
	size_t	i;

	assume((uintptr_t)depth % 128 == 0);
	assume((WIDTH * HEIGHT) % 32 == 0);
	i = 0;
	while (((uintptr_t)buffer + i) % 128)
		buffer[i++] = black;
	while (i < ((WIDTH * HEIGHT) / 2 - 16))
	{
		buffer[i] = black;//18.3 % runtime
		buffer[i + 1] = black;
		buffer[i + 2] = black;
		buffer[i + 3] = black;
		buffer[i + 4] = black;
		buffer[i + 5] = black;
		buffer[i + 6] = black;
		buffer[i + 7] = black;
		i += 8;
	}
	while (i < (WIDTH * HEIGHT / 2))
		buffer[i++] = black;
	i = 0;
	reset_depth(depth);
	//while (i < (WIDTH * HEIGHT))//2.5%runtime
	//{
	//	depth[i] = Z_FAR; //19.0% runtime
	//	depth[i + 1] = Z_FAR;
	//	depth[i + 2] = Z_FAR;
	//	depth[i + 3] = Z_FAR;
	//	depth[i + 4] = Z_FAR;
	//	depth[i + 5] = Z_FAR;
	//	depth[i + 6] = Z_FAR;
	//	depth[i + 7] = Z_FAR;
	//	i += 8;
	//}
}

// Print the window width and height.
void	ft_hook(void* param)
{
	t_main		*main_data = param;
	int i;

	if (menu_handler(main_data, &main_data->menu) == true)
		return ;
	if (main_data->settings.paused == true)
		return ;
	handle_movement_per_frame(main_data);
	reset_pixel_buffer_main(main_data->img->pixels, main_data->depth);
	i = 0;
	static int q = 0;
	while (i < main_data->mesh_count)
	{
		//mod_cube_rotation(main_data->meshes + i, main_data->mlx->delta_time);
		draw_mesh(main_data->meshes + i);

		if (!q)
		{
			printf("mesh %d has %d tris\n", i, main_data->meshes[i].count);
		}
		i++;
	}
	q++;
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
	tri->centroid.x = s.x / 3.0f;
	tri->centroid.y = s.y / 3.0f;
	tri->centroid.z = s.z / 3.0f;
}

void	add_obj_file_meshes(t_main *main_data)
{
	t_mesh	new_mesh;

	new_mesh = load_obj_file("./", "axis.obj", main_data);
	new_mesh.model_space_data.x_scale = 0.1;
	new_mesh.model_space_data.y_scale = 0.1;
	new_mesh.model_space_data.z_scale = 0.1;
	make_mesh_render_rdy(&new_mesh);
	if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
		ft_error(main_data);
	main_data->mesh_count++;
	//new_mesh = load_obj_file("teapot/", "teapot/teapot.obj", main_data);
	//new_mesh.model_space_data.x_scale = 0.1;
	//new_mesh.model_space_data.y_scale = 0.1;
	//new_mesh.model_space_data.z_scale = 0.1;

	//make_mesh_render_rdy(&new_mesh);
	//if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
	//	ft_error(main_data);
	//main_data->mesh_count++;
	
	//new_mesh = load_obj_file("./objs/", "objs/HorseArmor.obj", main_data);
	//make_mesh_render_rdy(&new_mesh);
	////new_mesh.model_space_data.x_scale = 20;
	////new_mesh.model_space_data.y_scale = 20;
	////new_mesh.model_space_data.z_scale = 20;
	//if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
	//	ft_error(main_data);
	//main_data->mesh_count++;

	//new_mesh = load_obj_file("./egg/", "egg/RAN_Easter_Egg_2024_High_Poly.obj", main_data);
	//make_mesh_render_rdy(&new_mesh);
	//new_mesh.model_space_data.x_scale = 10;
	//new_mesh.model_space_data.y_scale = 10;
	//new_mesh.model_space_data.z_scale = 10;
	//if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
	//	ft_error(main_data);
	//main_data->mesh_count++;

	//new_mesh = load_obj_file("./HP_Laptop_High_Poly/", "HP_Laptop_High_Poly/Laptop_HighPolay_HP_BI_2.obj", main_data);
	//make_mesh_render_rdy(&new_mesh);
	//if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
	//	ft_error(main_data);
	//main_data->mesh_count++;

	new_mesh = load_obj_file("./", "circle.obj", main_data);
	make_mesh_render_rdy(&new_mesh);
	if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
		ft_error(main_data);
	main_data->mesh_count++;

}

int32_t	main(void)
{
	mlx_image_t		*ob;
	static t_main			m_data; //has to be static so it's in the
                                   //BSS segment
	errno = 0;
	printf("FOV: %lf\n", FOV);
	printf("tan(FOV): %lf\n", tan(FOV));
	printf("FOV_X: %lf\nFOV_Y: %lf\n", FOV_X, FOV_Y);
	printf("FOV_RAD: %lf\n", FOV_RAD);
	printf("ASPECT_RATIO: %lf\n", ASPECT_RATIO);
	int i = 0;
	init_main(&m_data);
	add_obj_file_meshes(&m_data);
	//cleanup_exit(&m_data);
	init_hooks(&m_data);//leave here to find easier
	init_menu_widgets(&m_data);//leave here to find easier
	mlx_loop(m_data.mlx);
	cleanup_exit(&m_data);
	return (EXIT_SUCCESS);
}
//8352432
//448
