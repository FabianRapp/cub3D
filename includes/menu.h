/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:35:25 by frapp             #+#    #+#             */
/*   Updated: 2024/04/29 21:01:37 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

#define MENU_CLOSED 0
#define MENU_OPEN 1
#define MENU_OPENING 2
#define MENU_CLOSING 3

# include <MLX42.h>

// our headers
# include <cub3D.h>

#define MOUSE_SENS_ELEMENT 0

# define SLIDER_FILL_COL GREEN
# define SLIDER_TXT_COL BLACK

// --------------------------------------------
// depth levels , each text-/entry-field needs 2 lvls

#define SENS_DEPTH MENU_DEPTH + 1

// ---------------end depth levls-------------

typedef struct s_main	t_main;

typedef struct s_entry_field
{
	char			buffer[1024];
	bool			selected;
	mlx_image_t		*img;
	mlx_image_t		*title;
	uint32_t		xpos;
	uint32_t		ypos;
	uint32_t		height;
	uint32_t		width;
	float			val;
	bool			is_slider;
}	t_entry_field;

typedef struct s_menu
{
	uint8_t			state:2;
	mlx_image_t		*elements[10];
	mlx_image_t		*img;
	t_entry_field	mouse_sens;
	t_entry_field	*clicked_field;
}	t_menu;

bool	menu_handler(t_main *main_data);
void	free_menu(t_main *main_data);
void	fill_slider(t_entry_field *slider, float filling);

#endif