/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:26:19 by frapp             #+#    #+#             */
/*   Updated: 2024/05/01 14:58:08 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>
#include "menu_internals.h"

static t_entry_widget	*clicked_menu_field(t_main *main_data, int xpos, int ypos, t_menu *menu)
{
	t_entry_widget	*field;
	int				i;

	i = 0;
	while (i < menu->widget_count)
	{
		field = menu->all_widgets + i;
		if (xpos >= field->xpos && xpos < field->xpos + field->width
			&& ypos >= field->ypos && ypos < field->ypos + field->height)
		{
			return (field);
		}
	i++;
	}
	return (NULL);
}

void	menu_mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, t_main *main_data)
{
	t_entry_widget	*field;
	int				xpos;
	int				ypos;

	if (main_data->menu.state != MENU_OPEN)
		return ;
	mlx_get_mouse_pos(main_data->mlx, &xpos, &ypos);
	if (action == MLX_PRESS)
	{
		field = clicked_menu_field(main_data, xpos, ypos, &main_data->menu);
		if (!field)
			return ;
		main_data->menu.clicked_widget = field;
		return ;
	}
	else if (action == MLX_REPEAT || !main_data->menu.clicked_widget)
		return ;
	else if (action == MLX_RELEASE)
	{
		main_data->menu.clicked_widget = NULL;
		return ;
	}
}

// possible to highlight widget hovered over
void	cursor_menu(double xpos, double ypos, t_main *main_data, void fn(t_main *main_data, t_entry_widget *widget, int menu_index))
{
	t_menu	*menu;
	t_entry_widget	*widget;

	menu = &main_data->menu;
	if (menu->state != MENU_OPEN)
		return ;
	widget = menu->clicked_widget;
	if (!widget || widget->type != WIDGET_SLIDER)
		return ;
	if ((int)xpos <= widget->xpos)
		widget->val.slider_val = 0.001f;
	else if ((int)xpos >= widget->xpos + widget->width)
		widget->val.slider_val = 1.0f;
	else if ((int)xpos > widget->xpos)
	{
		widget->val.slider_val = (xpos - widget->xpos) / widget->width;
		widget->val.slider_val = fmax(0.0f, fmin(1.0f, widget->val.slider_val));
	}
	else
		ft_error(main_data);
	fn(main_data, widget, 0);
}
