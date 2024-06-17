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

#include <sanitizer/lsan_interface.h>

void	init_mlx_no_leaks(t_main *main_data)
{
	//__lsan_disable();
	main_data->mlx = mlx_init(WIDTH, HEIGHT, "cub3D", true);
	//__lsan_enable();
}

void	init_main(t_main *main_data)
{
	const t_vec3	init_cam = {.x = 0, .y = 0, .z = 0, .w = 1};
	const t_vec3	init_look_direct = {.x = 0, .y = 0, .z = 1, .w = 1};
	const t_vec3	init_up = {.x = 0.0, .y = 1, .z = 0, .w = 1};
	ft_bzero(main_data, sizeof(t_main));
	main_data->pitch = 0;
	main_data->yaw = 0;
	main_data->roll = 0;
	ft_memcpy(&main_data->camera, &init_cam, sizeof(init_cam));
	ft_memcpy(&main_data->up, &init_up, sizeof(init_up));
	unit_vec3(&main_data->up);
	ft_memcpy(&main_data->look_direct, &init_look_direct, sizeof(init_look_direct));
	unit_vec3(&main_data->look_direct);
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
	add_menu_widget(main_data, WIDGET_SLIDER, "Test 1", (t_widget_val)(main_data->settings.mouse_sens / MOUSE_SENS_BASE), &main_data->menu);
	add_menu_widget(main_data, WIDGET_SLIDER, "Press 'm' to exit/enter menu", (t_widget_val)(main_data->settings.mouse_sens / MOUSE_SENS_BASE), &main_data->menu);
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











