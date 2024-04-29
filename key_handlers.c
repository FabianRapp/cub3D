/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:30:59 by frapp             #+#    #+#             */
/*   Updated: 2024/04/29 18:13:22 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

void	wasd_key_handler(mlx_key_data_t keydata, void *param)
{
	t_controls	*controls;

	controls = &((t_main *)param)->controls;
	if (keydata.action == MLX_REPEAT)
	{
		if (keydata.key == FORWARD_KEY)
			controls->state.up = true;
		if (keydata.key == LEFT_KEY)
			controls->state.left = true;
		if (keydata.key == RIGHT_KEY)
			controls->state.right = true;
		if (keydata.key == BACKWARDS_KEY)
			controls->state.back = true;
	}
	else if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == FORWARD_KEY)
			controls->state.up = false;
		if (keydata.key == LEFT_KEY)
			controls->state.left = false;
		if (keydata.key == RIGHT_KEY)
			controls->state.right = false;
		if (keydata.key == BACKWARDS_KEY)
			controls->state.back = false;
	}
}

void	jump_key_handler(mlx_key_data_t keydata, void *param)
{
	t_controls	*controls;

	controls = &((t_main *)param)->controls;
	if (keydata.key == JUMP_KEY && keydata.action != MLX_RELEASE)
	{
		controls->state.jump = true;
	}
	else
	{
		controls->state.jump = false;
	}
	if (keydata.key == NEGATIVE_JUMP_KEY && keydata.action != MLX_RELEASE)
		controls->state.negative_jump = true;
	else
		controls->state.negative_jump = false;
}

void	settings_key_handler(mlx_key_data_t keydata, t_main *main_data)
{
	if (keydata.key == CURSORE_MODE_TOGGLE && keydata.action == MLX_PRESS)
	{
		if (main_data->settings.cursor_lock)
		{
			//main_data->settings.cursor_lock = false;
			main_data->settings.cursor_hide = false;
			mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
		}
		else
		{
			main_data->settings.cursor_lock = true;
			main_data->settings.cursor_hide = true;
			mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_HIDDEN);
		}
	}
	else if (keydata.key == PAUSE_KEY && keydata.action == MLX_PRESS)
	{
		if (main_data->settings.paused == true)
			main_data->settings.paused = false;
		else
			main_data->settings.paused = true;
	}
}

void	toggl_menu_state(t_main *main_data)
{
	if (main_data->settings.menu_state == MENU_OPENING || main_data->settings.menu_state == MENU_CLOSING)
		return ;
	else if (main_data->settings.menu_state == MENU_CLOSED)
		main_data->settings.menu_state = MENU_OPENING;
	else if (main_data->settings.menu_state == MENU_OPEN)
		main_data->settings.menu_state = MENU_CLOSING;
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	if (keydata.key == FORWARD_KEY || keydata.key == LEFT_KEY || keydata.key == BACKWARDS_KEY || keydata.key == RIGHT_KEY)
		wasd_key_handler(keydata, param);
	if (keydata.key == JUMP_KEY || keydata.key == NEGATIVE_JUMP_KEY)
		jump_key_handler(keydata, param);
	if (keydata.key == CURSORE_MODE_TOGGLE || keydata.key == PAUSE_KEY)
		settings_key_handler(keydata, (t_main *)param);
	if (keydata.key == MENU_KEY && keydata.action == MLX_PRESS)
		toggl_menu_state((t_main *)param);
}

void	cursor_hook(double xpos, double ypos, void* param)
{
	float			x_dist;
	float			y_dist;
	t_main			*main_data;

	main_data = (t_main *)param;
	x_dist = xpos - WIDTH / 2;
	y_dist = ypos - HEIGHT / 2;
	// x_dist -= WIDTH / 2 - main_data->controls.last_mouse_pos[X];
	// y_dist -= HEIGHT / 2 - main_data->controls.last_mouse_pos[Y];
	printf("x_dist : %f y_dist: %f\n", x_dist, y_dist);
	// main_data->pitch += ((float)M_PI * MOUSE_SENS_BASE) * y_dist;
	// main_data->yaw += ((float)M_PI * MOUSE_SENS_BASE) * x_dist;
	main_data->pitch += main_data->settings.mouse_sens * y_dist;
	main_data->yaw += main_data->settings.mouse_sens * x_dist;
	main_data->look_direct = v3_add(main_data->look_direct, get_direction(main_data->pitch, main_data->yaw, main_data->roll));
	unit_vec3(&main_data->look_direct);
	if (main_data->settings.cursor_lock)
	{
		mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);
	}
}
