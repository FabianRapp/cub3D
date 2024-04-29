/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 18:49:11 by frapp             #+#    #+#             */
/*   Updated: 2024/04/29 21:04:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>

void	free_entry_field(t_main *main_data, t_entry_field *field)
{
	ft_free_img(main_data, &field->img);
	ft_free_img(main_data, &field->title);
}

void	free_menu(t_main *main_data)
{
	t_menu	*menu;

	if (main_data->settings.cursor_lock)
		mlx_set_mouse_pos(main_data->mlx, WIDTH / 2, HEIGHT / 2);
	if (main_data->settings.cursor_hide)
		mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_HIDDEN);
	else
		mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
	main_data->menu.state = MENU_CLOSED;
	menu = &main_data->menu;
	menu->clicked_field = NULL;
	ft_free_img(main_data, &menu->img);
	ft_free_img(main_data, &menu->elements[MOUSE_SENS_ELEMENT]);
	free_entry_field(main_data, &menu->mouse_sens);
}

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

void	make_entry_field(t_main *main_data, t_entry_field *field, char *title, int depth)
{
	int		i;

	field->selected = false;
	free_entry_field(main_data, field);
	field->title = mlx_put_string(main_data->mlx, title, field->xpos, field->ypos);
	if (!field->title)
		ft_error(main_data);
	mlx_set_instance_depth(field->title->instances, depth + 1);
	recolor_text_in_field(field->title, SLIDER_TXT_COL);
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

void	fill_slider(t_entry_field *slider, float filling)
{
	int	cur_x;
	int	cur_y;

	if (filling > 1)
	{
		fprintf(stderr, "error fill_slider\n");
		exit(1);
	}
	cur_y = 0;
	slider->val = filling;
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

void	make_mouse_sens_field(t_main *main_data)
{
	t_menu			*menu;
	t_entry_field	*field;

	menu = &main_data->menu;
	field = &menu->mouse_sens;
	field->is_slider = true;
	field->xpos = WIDTH / 10;
	field->ypos = HEIGHT / 2;
	field->width = WIDTH * 8 / 10;
	field->height = HEIGHT / 18;
	make_entry_field(main_data, field, "Mouse Sensibility", SENS_DEPTH);
	// 
	fill_slider(field, main_data->settings.mouse_sens);


}

void	create_menu_img(t_main *main_data)
{
	t_menu	*menu;

	menu = &main_data->menu;
	free_menu(main_data);
	mlx_set_cursor_mode(main_data->mlx, MLX_MOUSE_NORMAL);
	main_data->menu.state = MENU_OPEN;
	menu->img = mlx_new_image(main_data->mlx, WIDTH, HEIGHT);
	if (!menu->img || mlx_image_to_window(main_data->mlx, menu->img, 0, 0))
		ft_error(main_data);
	reset_pixel_buffer(menu->img->pixels, NULL);
	mlx_set_instance_depth(menu->img->instances, MENU_DEPTH);
	//menu->elements[MOUSE_SENS_ELEMENT] = mlx_put_string(main_data->mlx, "TEST", WIDTH / 2, HEIGHT / 2);
	make_mouse_sens_field(main_data);
}

bool	menu_handler(t_main *main_data)
{
	t_menu	*menu;

	menu = &main_data->menu;
	if (menu->state == MENU_CLOSED)
		return (false);
	else if (menu->state == MENU_OPEN)
	{
		fill_slider(&menu->mouse_sens, menu->mouse_sens.val);
		return (true);
	}
	else if (menu->state == MENU_OPENING)
		create_menu_img(main_data);
	else if (menu->state == MENU_CLOSING)
		free_menu(main_data);
	return (true);
}
