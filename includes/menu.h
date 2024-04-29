/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menu.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/29 16:35:25 by frapp             #+#    #+#             */
/*   Updated: 2024/04/29 16:38:18 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MENU_H
# define MENU_H

#define MENU_CLOSED 0
#define MENU_OPEN 1
#define MENU_OPENING 2
#define MENU_CLOSING 3

// our headers
# include <cub3D.h>

typedef struct s_menu
{
	mlx_image_t	*img;
}	t_menu;

#endif