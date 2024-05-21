/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:30:59 by frapp             #+#    #+#             */
/*   Updated: 2024/05/21 17:25:38 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>

void	wasd_key_handler(mlx_key_data_t keydata, void *param)
{
	t_controls	*controls;

	controls = &((t_main *)param)->controls;
	if (keydata.action != MLX_RELEASE)
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
			main_data->settings.cursor_lock = false;
			main_data->settings.cursor_hide = false;
			mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
		}
		else
		{
			main_data->settings.cursor_lock = true;
			//main_data->settings.cursor_hide = true;
			//mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_HIDDEN);
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
	if (main_data->menu.state == MENU_OPENING || main_data->menu.state == MENU_CLOSING)
		return ;
	else if (main_data->menu.state == MENU_CLOSED)
		main_data->menu.state = MENU_OPENING;
	else if (main_data->menu.state == MENU_OPEN)
		main_data->menu.state = MENU_CLOSING;
}

void	key_hook_menu(mlx_key_data_t keydata, t_main *main_data)
{
	if (main_data->menu.state != MENU_OPEN)
		return ;
	if (keydata.key == MENU_KEY && keydata.action == MLX_PRESS)
	{
		toggl_menu_state(main_data);
		return ;
	}
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	if (((t_main *)param)->menu.state != MENU_CLOSED)
	{
		key_hook_menu(keydata, (t_main *)param);
		return ;
	}
	if (keydata.key == FORWARD_KEY || keydata.key == LEFT_KEY || keydata.key == BACKWARDS_KEY || keydata.key == RIGHT_KEY)
		wasd_key_handler(keydata, param);
	if (keydata.key == JUMP_KEY || keydata.key == NEGATIVE_JUMP_KEY)
		jump_key_handler(keydata, param);
	if (keydata.key == CURSORE_MODE_TOGGLE || keydata.key == PAUSE_KEY)
		settings_key_handler(keydata, (t_main *)param);
	if (keydata.key == MENU_KEY && keydata.action == MLX_PRESS)
		toggl_menu_state((t_main *)param);
}

void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param)
{
	t_main	*main_data;

	main_data = (t_main *)param;
	if (main_data->menu.state != MENU_CLOSED)
	{
		menu_mouse_hook(button, action, mods, main_data);
		return ;
	}
}

void	menu_action(t_main *main_data, t_entry_widget *widget, int menu_index)
{
	if (menu_index == 0 && widget->index == MOUSE_SENS_WIDGET_INDEX)
		main_data->settings.mouse_sens = powf(widget->val.slider_val, 0.5) * MOUSE_SENS_BASE;
}

void	cursor_hook(double xpos, double ypos, void* param)
{
	float			x_dist;
	float			y_dist;
	t_main			*main_data;

	main_data = (t_main *)param;
	if (main_data->menu.state != MENU_CLOSED)
	{
		cursor_menu(xpos, ypos, main_data, menu_action);
		return ;
	}
	x_dist = xpos - WIDTH / 2;
	y_dist = ypos - HEIGHT / 2;
	main_data->pitch += main_data->settings.mouse_sens * y_dist;
	if (main_data->pitch < ((-M_PI + 0.05) / 2))
		main_data->pitch = ((-M_PI + 0.05) / 2);
	else if (main_data->pitch > ((M_PI - 0.05)/ 2))
		main_data->pitch = ((M_PI - 0.05) / 2);
	main_data->yaw += main_data->settings.mouse_sens * x_dist;
	main_data->yaw -= ((int)(main_data->yaw / (2 * (float)M_PI)))* (2 * (float)M_PI); // main_data->yaw %= 2 * PI
	//printf("pitch: %f\nyaw: %f\n", main_data->pitch, main_data->yaw);
	main_data->look_direct = v3_add(main_data->look_direct, get_direction(main_data->pitch, main_data->yaw, main_data->roll));
	unit_vec3(&main_data->look_direct);
	if (main_data->settings.cursor_lock)
	{
		mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);
	}
}


