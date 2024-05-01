/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu_internals.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/01 14:21:36 by frapp             #+#    #+#             */
/*   Updated: 2024/05/01 14:33:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_INTERNALS_H
# define MENU_INTERNALS_H

#include <cub3D.h>
#include <MLX42.h>
#include <menu.h>

void	close_menu(t_main *main_data, t_menu *menu);
void	open_menu(t_main *main_data);
void	handle_open_menu(t_main *main_data, t_menu *menu);
void	init_entry_widget(t_main *main_data, t_entry_widget *field, char *title, int depth);
void	fill_slider(t_entry_widget *slider, float filling);

#endif
