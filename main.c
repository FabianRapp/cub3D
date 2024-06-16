/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/05/26 03:47:36 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "includes/menu.h"

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
		t_vec3 left = cross_product(main_data->look_direct, main_data->up);
		unit_vec3(&left);
		main_data->camera.x += main_data->mlx->delta_time * controls.movement_speed_left * left.x;
		main_data->camera.z += main_data->mlx->delta_time * controls.movement_speed_left * left.z;
	}
	if (controls.state.right)
	{
		t_vec3 right = cross_product(main_data->up, main_data->look_direct);
		unit_vec3(&right);
		main_data->camera.x += main_data->mlx->delta_time * controls.movement_speed_right * right.x;
		main_data->camera.z += main_data->mlx->delta_time * controls.movement_speed_right * right.z;
	}
	if (controls.state.back)
	{
		movement.x -= main_data->look_direct.x * main_data->mlx->delta_time * controls.movement_speed_back;
		movement.y -= main_data->look_direct.y * main_data->mlx->delta_time * controls.movement_speed_back;
		movement.z -= main_data->look_direct.z * main_data->mlx->delta_time * controls.movement_speed_back;
	}
	if (controls.state.jump)
		movement.y -= controls.jump_height * main_data->mlx->delta_time;
	if (controls.state.negative_jump)
		movement.y += controls.jump_height * main_data->mlx->delta_time;
	if (!zero_f(movement.x) || !zero_f(movement.y) || !zero_f(movement.z))
		add_vec3(&main_data->camera, &movement);
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
	ident_mat_4x4(main_data->world_mat);
	reset_pixel_buffer(main_data->img->pixels, main_data->depth);
	i = 0;
	while (i < main_data->mesh_count)
	{
		//mod_cube_rotation(main_data->meshes + i, main_data->mlx->delta_time);
		draw_mesh(main_data->meshes + i);
		i++;
	}
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

void	free_mesh(t_mesh *mesh)
{
	int	i;

	i = 0;
	while (i < mesh->mtl_count)
	{
		i++;
	}
	free(mesh->mtl_libs);
	ft_free((void **)&mesh->triangles);
}

void	clean_mlx(t_main *main_data)
{
	if (main_data->mlx)
	{
		mlx_close_window(main_data->mlx);
		free_fps_digit_textures();
		if (main_data->img)
			mlx_delete_image(main_data->mlx, main_data->img);
		mlx_terminate(main_data->mlx);
		main_data->mlx = NULL;
		//printf("on linux MLX will always leak 12140 byte(s) leaked in 81 allocation(s) (with fsanitize)\n");
	}
}

void	cleanup_exit(void *m_data)
{
	t_main	*main_data;
	int		i;

	main_data = (t_main *)m_data;
	fprintf(stderr, "exiting..\n");
	i = 0;
	while (i < main_data->mesh_count)
	{
		free_mesh(main_data->meshes + i);
		i++;
	}
	ft_free((void **)&main_data->meshes);
	free_menu(main_data, &main_data->menu);
	clean_mlx(main_data);
	exit(0);
}

void	add_obj_file_meshes(t_main *main_data)
{
	t_mesh	new_mesh;

	new_mesh = load_obj_file("./", "axis.obj", main_data);
	if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
		ft_error(main_data);
	main_data->mesh_count++;
	new_mesh = load_obj_file("teapot/", "teapot/teapot.obj", main_data);
	if (!arr_append((void **)(&main_data->meshes), &new_mesh, sizeof(t_mesh), main_data->mesh_count))
		ft_error(main_data);
	main_data->mesh_count++;
}

int32_t	main(void)
{
	mlx_image_t		*ob;
	static t_main			m_data; //has to be static so it's in the
                                    //BSS segment

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
