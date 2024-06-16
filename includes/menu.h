/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:35:25 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

/*
	TODO:
	- make a menu array in the t_main struct to allow diffrent options + supporting function changes
	- make widgets modify a mem locatation (void *) given in init rather than struct var: remove need to take call back fn
*/

#define MENU_CLOSED 0
#define MENU_OPEN 1
#define MENU_OPENING 2
#define MENU_CLOSING 3

# include "../MLX42/include/MLX42/MLX42.h"

// our headers
# include "cub3D.h"

#define MOUSE_SENS_ELEMENT 0

# define SLIDER_FILL_COL GREEN
# define SLIDER_TXT_COL BLACK

// --------------------------------------------
// depth levels , each text-/entry-field needs 2 lvls

#define SENS_DEPTH MENU_DEPTH + 1

// ---------------end depth levls-------------

// ---------t_entry_widget parameters --------------------
#define WIDGET_WIDTH (WIDTH / 10 * 8)
#define WIDGET_HEIGHT (HEIGHT / 18)
#define WIDGET_X_POS (WIDTH / 10)
// ---------------------------------------------------------

typedef struct s_main	t_main;

//widget types
#define WIDGET_SLIDER 1

//----------------widget init_data----
typedef union u_widget_val
{
	double	slider_val;
	int32_t	int_val;
}	t_widget_val;

typedef struct s_entry_widget
{
	//public
	uint16_t		index;
	uint8_t			type;
	char			title[1024];
	bool			selected;
	uint32_t		xpos;
	uint32_t		ypos;
	mlx_image_t		*img;
	mlx_image_t		*title_img;
	uint32_t		height;
	uint32_t		width;
	t_widget_val	val;
	//private
}	t_entry_widget;

typedef struct s_menu
{
	//public
	t_entry_widget	*all_widgets;
	uint16_t		widget_count;
	//private
	mlx_image_t		*img;
	uint8_t			state:2;
	t_entry_widget	*clicked_widget;
}	t_menu;

// engine iterface to user
void			free_menu(t_main *main_data, t_menu *menu);
t_entry_widget	*add_menu_widget(t_main *main_data, int8_t type, char *title, t_widget_val init_val, t_menu *menu);
void			get_widget_val(t_menu *menu, int widget_index, t_widget_val *val);

// internal engine interface
void			init_menu(t_main *main_data, t_menu *menu);
bool			menu_handler(t_main *main_data, t_menu *menu);
void			menu_mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, t_main *main_data);
void			cursor_menu(double xpos, double ypos, t_main *main_data, void fn(t_main *main_data, t_entry_widget *widget, int menu_index));

#endif
