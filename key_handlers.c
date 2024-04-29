/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 15:30:59 by frapp             #+#    #+#             */
/*   Updated: 2024/04/29 22:23:45 by frapp            ###   ########.fr       */
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

t_entry_field	*clicked_menu_field(t_main *main_data, int xpos, int ypos)
{
	t_entry_field	*field;
	

	field = &main_data->menu.mouse_sens;
	if (xpos >= field->xpos && xpos < field->xpos + field->width
		&& ypos >= field->ypos && ypos < field->ypos + field->height)
	{
		return (field);
	}
	return (NULL);
}

void	menu_mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, t_main *main_data)
{
	t_entry_field	*field;
	int				xpos;
	int				ypos;

	if (main_data->menu.state != MENU_OPEN)
		return ;
	mlx_get_mouse_pos(main_data->mlx, &xpos, &ypos);
	if (action == MLX_PRESS)
	{
		field = clicked_menu_field(main_data, xpos, ypos);
		if (!field)
			return ;
		main_data->menu.clicked_field = field;
		return ;
	}
	else if (action == MLX_REPEAT || !main_data->menu.clicked_field)
		return ;
	else if (action == MLX_RELEASE)
	{
		main_data->menu.clicked_field = NULL;
		return ;
	}
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

void	cursor_menu(double xpos, double ypos, t_main *main_data)
{
	t_menu	*menu;
	t_entry_field	*field;

	menu = &main_data->menu;
	if (menu->state != MENU_OPEN)
		return ;
	field = menu->clicked_field;
	if (field)
	{
		if (field->is_slider)
		{
			if ((int)xpos <= field->xpos)
			{
				field->val = 0.001f;
			}
			else if ((int)xpos >= field->xpos + field->width)
			{
				field->val = 1.0f;
			}
			else if ((int)xpos > field->xpos)
			{
				field->val = (xpos - field->xpos) / field->width;
				field->val = fmax(0.0f, fmin(1.0f, field->val));
			}
			else
			{
				ft_error(main_data);
			}
			if (field->img == main_data->menu.mouse_sens.img)
			{
				//field->val is a slider val beteen 0.0001 and 1.0
				//main_data->settings.mouse_sens = field->val * MOUSE_SENS_BASE; //tring to avoid linear calc
				main_data->settings.mouse_sens = powf(field->val, 0.5) * MOUSE_SENS_BASE;
				printf("mouse sens: %f\n", main_data->settings.mouse_sens);
			}
			
		}
	}
}

void	cursor_hook(double xpos, double ypos, void* param)
{
	float			x_dist;
	float			y_dist;
	t_main			*main_data;

	main_data = (t_main *)param;
	if (main_data->menu.state != MENU_CLOSED)
	{
		cursor_menu(xpos, ypos, main_data);
		return ;
	}
	x_dist = xpos - WIDTH / 2;
	y_dist = ypos - HEIGHT / 2;
	main_data->pitch += main_data->settings.mouse_sens * y_dist;
	main_data->yaw += main_data->settings.mouse_sens * x_dist;
	main_data->look_direct = v3_add(main_data->look_direct, get_direction(main_data->pitch, main_data->yaw, main_data->roll));
	unit_vec3(&main_data->look_direct);
	if (main_data->settings.cursor_lock)
	{
		mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);
	}
}
