/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 17:51:26 by frapp             #+#    #+#             */
/*   Updated: 2024/05/26 03:14:43 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"
#include "includes/menu.h"
#include "includes/libft.h"

static void	init_cursor(t_main *main_data)
{
	mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_DISABLED);
	main_data->settings.cursor_lock = true;
	main_data->settings.cursor_hide = true;
	main_data->settings.mouse_sens = 0.5 * MOUSE_SENS_BASE;
	mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
	mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);// does not work before loop
}

static void	init_settings(t_main *main_data)
{
	main_data->settings.paused = false;
	main_data->controls.movement_speed_straight = 1;
	main_data->controls.movement_speed_left = 1;
	main_data->controls.movement_speed_right = 1;
	main_data->controls.movement_speed_back = 1;
	main_data->controls.jump_height = 1;
}


void	init_mlx_no_leaks(t_main *main_data)
{
	//__lsan_disable();
	main_data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	//__lsan_enable();
}

void	init_main(t_main *main_data)
{
	ft_bzero(main_data, sizeof(t_main));
	reset_camera(main_data);
	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(0, true);
	init_mlx_no_leaks(main_data);
	if (!main_data->mlx)
		ft_error(main_data);
	main_data->img = mlx_new_image(main_data->mlx, WIDTH, HEIGHT);
	if (!main_data->img || (mlx_image_to_window(main_data->mlx, main_data->img, 0, 0) < 0))
		ft_error(main_data);
	init_cursor(main_data);
	get_fps_digit_texture();
	init_settings(main_data);
	init_menu(main_data, &main_data->menu);
	mlx_set_instance_depth(main_data->img->instances, MAIN_RENDER_DEPTH);
}

void	init_hooks(t_main *main_data)
{
	mlx_mouse_hook(main_data->mlx, mouse_hook, main_data); // first
	mlx_key_hook(main_data->mlx, &key_hook, main_data); // second
	mlx_cursor_hook(main_data->mlx, cursor_hook, main_data); // third
	mlx_loop_hook(main_data->mlx, display_fps_hook, main_data->mlx); // dosnt matter when
	mlx_loop_hook(main_data->mlx, ft_hook, main_data); // last
	mlx_close_hook(main_data->mlx, cleanup_exit, main_data); // dosnt matter when
}

void	init_menu_widgets(t_main *main_data)
{
	add_menu_widget(main_data, WIDGET_SLIDER, "Mouse Sensibility", (t_widget_val)(main_data->settings.mouse_sens / MOUSE_SENS_BASE), &main_data->menu);
	//add_menu_widget(main_data, WIDGET_SLIDER, "Test 1", (t_widget_val)(main_data->settings.mouse_sens / MOUSE_SENS_BASE), &main_data->menu);
	add_menu_widget(main_data, WIDGET_SLIDER, "Press 'm' to exit/enter menu", (t_widget_val)(main_data->settings.mouse_sens / MOUSE_SENS_BASE), &main_data->menu);
}

// called before a mesh is added to the redender loop and after
// initiializing it
// can throw errors in debug mode for invalid data
void	make_mesh_render_rdy(t_mesh *mesh)
{
	//int	i;
	//int	j;
	//t_triangle	*tris;

	//tris = mesh->triangles;
	//i = 0;
	//while (i < mesh->count)
	//{
	//	j = 0;
	//	while (j < 3)
	//	{
	//		j++;
	//	}
	//	i++;
	//}
}


void	init_default_model_space(t_model_space_data *data)
{
	ident_mat_4x4(data->model_matrix);
	data->x_scale = 1.0;
	data->y_scale = 1.0;
	data->z_scale = 1.0;
	data->x_translation = 0.0;
	data->y_translation = 0.0;
	data->z_translation = 0.0;
	data->x_rotation = 0.0;
	data->y_rotation = 0.0;
	data->z_rotation = 0.0;
}

void	init_default_physics_data(t_main *main_data, t_physics_data *data)
{
	data->delta_time = &main_data->mlx->delta_time;
	data->x_speed = 0;
	data->y_speed = 0;
	data->z_speed = 0;
}

//needs mlx to be initialized first
void	init_basic_data_mesh(t_main *main_data, t_mesh *mesh)
{
	ft_bzero(mesh, sizeof *mesh);
	init_default_model_space(&mesh->model_space_data);
	init_default_physics_data(main_data, &mesh->physics_data);
	mesh->world_data = &main_data->world_data;
}








