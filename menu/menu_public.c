/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_public.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:22:47 by frapp             #+#    #+#             */
/*   Updated: 2024/05/01 14:52:24 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>
#include "menu_internals.h"

void	free_menu(t_main *main_data, t_menu *menu)
{
	int	i;

	ft_free_img(main_data, &menu->img);
	i = 0;
	while (i < menu->widget_count)
	{
		ft_free_img(main_data, &menu->all_widgets[i].title_img);
		ft_free_img(main_data, &menu->all_widgets[i].img);
		i++;
	}
	free(menu->all_widgets);
	ft_bzero(menu, sizeof(t_menu));
	menu->state = MENU_CLOSED;
}

bool	menu_handler(t_main *main_data, t_menu *menu)
{
	if (menu->state == MENU_CLOSED)
		return (false);
	else if (menu->state == MENU_OPEN)
	{
		handle_open_menu(main_data, menu);
		//fill_slider(&menu->mouse_sens, menu->mouse_sens.val);
		return (true);
	}
	else if (menu->state == MENU_OPENING)
		open_menu(main_data);
	else if (menu->state == MENU_CLOSING)
		close_menu(main_data, menu);
	return (true);
}

void	init_menu(t_main *main_data, t_menu	*menu)
{
	ft_bzero(menu, sizeof(t_menu));
	menu->state = MENU_OPEN;
	menu->img = mlx_new_image(main_data->mlx, WIDTH, HEIGHT);
	if (!menu->img || mlx_image_to_window(main_data->mlx, menu->img, 0, 0))
		ft_error(main_data);
	menu->img->enabled = true;
	reset_pixel_buffer(menu->img->pixels, NULL);
	mlx_set_instance_depth(menu->img->instances, MENU_DEPTH);
}

t_entry_widget	*add_menu_widget(t_main *main_data, int8_t type, char *title, t_widget_val init_val, t_menu *menu)
{
	t_entry_widget	*reallocked_widgets;
	t_entry_widget	*new_widget;

	reallocked_widgets = ft_calloc(menu->widget_count + 1, sizeof(t_entry_widget));
	if (!reallocked_widgets)
		ft_error(main_data);
	ft_memcpy(reallocked_widgets, menu->all_widgets, sizeof(t_entry_widget) * menu->widget_count);
	free(menu->all_widgets);
	new_widget = reallocked_widgets + menu->widget_count;
	menu->all_widgets = reallocked_widgets;
	new_widget->type = type;
	new_widget->index = menu->widget_count;
	menu->widget_count += 1;
	new_widget->xpos = WIDGET_X_POS;
	new_widget->width = WIDGET_WIDTH;
	new_widget->height = WIDGET_HEIGHT;
	ft_strlcpy(new_widget->title, title, sizeof(new_widget->title));
	if (new_widget->index)
	{
		new_widget->ypos = menu->all_widgets[new_widget->index - 1].ypos + WIDGET_HEIGHT * 2;
	}
	else
	{
		new_widget->ypos = WIDGET_HEIGHT * 2;
	}
	init_entry_widget(main_data, new_widget, title, MENU_DEPTH + 1 + new_widget->index * 2);
	if (type == WIDGET_SLIDER)
	{
		fill_slider(new_widget, init_val.slider_val);
	}
	return (new_widget);
}

void	get_widget_val(t_menu *menu, int widget_index, t_widget_val *val)
{
	int	i;

	i = 0;
	while (i < menu->widget_count)
	{
		if (i == widget_index)
			*val = menu->all_widgets[i].val;
		i++;
	}
}
