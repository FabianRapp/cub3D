/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:49:11 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../MLX42/include/MLX42/MLX42.h"
#include "../includes/menu.h"
#include "menu_internals.h"

void	recolor_text_in_field(mlx_image_t *img, uint32_t col)
{
	int			x;
	int			y;
	uint32_t	*pixels;
	int			width;
	int			height;

	width = img->width;
	height = img->height;
	pixels = (uint32_t *)img->pixels;
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			if (((t_color_split) (pixels[x + y * width])).argb[A] > 150)
			{
				pixels[x + y * width] = col;
			}
			else
				((t_color_split) (pixels[x + y * width])).argb[A] = 0;
			x++;
		}
		y++;
	}
}

void	init_entry_widget(t_main *main_data, t_entry_widget *field, char *title, int depth)
{
	int		i;

	field->selected = false;
	field->title_img = mlx_put_string(main_data->mlx, title, field->xpos, field->ypos);
	if (!field->title_img)
		ft_error(main_data);
	
	mlx_set_instance_depth(field->title_img->instances, depth + 1);
	recolor_text_in_field(field->title_img, SLIDER_TXT_COL);
	field->img = mlx_new_image(main_data->mlx, field->width, field->height);
	if (!field->img)
		ft_error(main_data);
	i = 0;
	while (i < field->width * field->height)
	{
		((uint32_t *) field->img->pixels)[i] = DARK_GREY;
		i++;
	}
	if (mlx_image_to_window(main_data->mlx, field->img, field->xpos, field->ypos))
		ft_error(main_data);
	mlx_set_instance_depth(field->img->instances, depth);
}

void	fill_slider(t_entry_widget *slider, double filling)
{
	int	cur_x;
	int	cur_y;

	if (filling > 1)
	{
		fprintf(stderr, "error fill_slider\n");
		exit(1);
	}
	static int i = 0;
	cur_y = 0;
	slider->val.slider_val = filling;
	while (cur_y < slider->height)
	{
		cur_x = 0;
		while (cur_x < ((int) slider->width * filling))
		{
			((uint32_t *) slider->img->pixels)[cur_x + cur_y * slider->width] = SLIDER_FILL_COL;
			cur_x++;
		}
		while (cur_x < ((int) slider->width))
		{
			((uint32_t *) slider->img->pixels)[cur_x + cur_y * slider->width] = LIGHT_GREY;
			cur_x++;
		}
		cur_y++;
	}
}

void	handle_open_menu(t_main *main_data, t_menu *menu)
{
	int	i;

	i = 0;
	while (i < menu->widget_count)
	{
		if (menu->all_widgets[i].type == WIDGET_SLIDER)
		{
			fill_slider(menu->all_widgets + i, menu->all_widgets[i].val.slider_val);
		}
		i++;
	}
}
