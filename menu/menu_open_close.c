/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_open_close.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:20:31 by frapp             #+#    #+#             */
/*   Updated: 2024/05/01 14:31:34 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>
#include "menu_internals.h"

void	close_menu(t_main *main_data, t_menu *menu)
{
	int	i;

	if (main_data->settings.cursor_lock)
		mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);
	if (main_data->settings.cursor_hide)
		mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_HIDDEN);
	else
		mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
	menu->img->enabled = false;
	menu->state = MENU_CLOSED;
	menu->clicked_widget = NULL;
	i = 0;
	while (i < menu->widget_count)
	{
		menu->all_widgets[i].img->enabled = false;
		menu->all_widgets[i].title_img->enabled = false;
		i++;
	}
}

void	open_menu(t_main *main_data)
{
	t_menu	*menu;
	int		i;

	menu = &main_data->menu;
	mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
	main_data->menu.state = MENU_OPEN;
	menu->img->enabled = true;
	menu->state = MENU_OPEN;
	menu->clicked_widget = NULL;
	i = 0;
	while (i < menu->widget_count)
	{
		menu->all_widgets[i].img->enabled = true;
		menu->all_widgets[i].title_img->enabled = true;
		i++;
	}
}

