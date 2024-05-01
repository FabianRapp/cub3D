/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/04/29 22:47:53 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

t_vec3	line_plane_intersection(t_vec3 plane_p, t_vec3 plane_n, t_vec3 line_start, t_vec3 line_end)
{
	t_vec3	intersection;
	t_vec3	line_direct;

	line_direct = v3_sub(line_end, line_start);
	unit_vec3(&line_direct);
	unit_vec3(&plane_p);
	intersection.z = -1;
	if (line_direct.x == plane_p.x && line_direct.y == plane_p.y)
		return (intersection);
	//plane_p, 
}

void	clipping_back(t_triangle *tri)
{
	
}

void	clipping_front(t_triangle *tri)
{
	
}

void	clipping_main(t_triangle *tri)
{
	
}
