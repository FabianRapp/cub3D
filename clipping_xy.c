/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/06 12:25:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

/*
	if base_flags[0] -> l1 goes through (WIDTH, HEIGHT)
	if !base_flags[0] -> l1 goes through (0, 0)
	if base_flags[1] -> l1 is along the y axis
	if !base_flags[1] -> l1 is along the x axis
	(l1 is either the x or the y axis)
	returns the multiplier for the direct where direct * x + p == p on l1
*/
void	fast_line_intersect(const int8_t base_flags[2], t_vec3 p, t_vec3 *direct)
{
	float	progress;

	if (base_flags[0])
	{
		if (base_flags[1])
			progress = (HEIGHT / direct->y - p.y / direct->y);
		else
			progress = (WIDTH / direct->x - p.x / direct->x);
	}
	else
	{
		if (base_flags[1])
			progress = (-p.y / direct->y);
		else
			progress = (-p.x / direct->x);
	}
	direct->x = direct->x * progress;
	direct->y = direct->y * progress;
	direct->u = (direct->u - p.u) * progress;
	direct->v = (direct->v - p.v) * progress;
}

int	clipping_left(t_triangle *tri, t_triangle *clipped)
{
	int8_t				i;
	int8_t				inside_points;
	int8_t				inside_index[3];
	static const int8_t	flags[2] = {0, 0};

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (tri->p[i].x >= 0)
			inside_index[inside_points++] = i;
		i++;
	}
	if (inside_points == 0)
		return (0);
	if (inside_points == 3)
	{
		*clipped = *tri;
		return (1);
	}
	if (inside_points == 1)
	{
		clipped[0] = *tri;
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		fast_line_intersect(flags, tri->p[inside_index[0]], tri->p + outside_index1);
		fast_line_intersect(flags, tri->p[inside_index[0]], tri->p + outside_index2);
		return (1);
	}
	// this is super inefficent (alot of area is covered twice (draw the tris))
	clipped[0] = *tri;
	clipped[1] = *tri;
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	// TODO i think these triangles are wrong or atleast bad
	// clipped[0].p[outside_index] = line_plane_intersection(plane_p, plane_n, tri->p[inside_index[0]], tri->p[outside_index]);
	// clipped[1].p[outside_index] = line_plane_intersection(plane_p, plane_n, tri->p[inside_index[1]], tri->p[outside_index]);
	return (2);
}

void	clipping_right()
{
}

void	clipping_up()
{
	
}

void	clipping_down()
{
	
}