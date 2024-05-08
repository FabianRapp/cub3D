/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/08 23:03:07 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

/*
	if base_flags[0] -> l1 goes through (WIDTH, HEIGHT)
	if !base_flags[0] -> l1 goes through (0, 0)
	if base_flags[1] -> l1 is along the y axis
	if !base_flags[1] -> l1 is along the x axis
*/
void	fast_line_intersect(const int8_t base_flags[2], t_vec3 p1, t_vec3 *p2)
{
	float	progress;

	if (base_flags[0])
	{
		if (base_flags[1])
			progress = ((HEIGHT - p1.y) / (p2->y - p1.y));
		else
			progress = ((WIDTH -p1.x) / (p2->x - p1.x));
	}
	else
	{
		if (base_flags[1])
			progress = (-p1.y / (p2->y - p1.y));
		else
			progress = (-p1.x / (p2->x - p1.x));
	}
	p2->x = p1.x + (p2->x - p1.x) * progress;
	p2->y = p1.y + (p2->y - p1.y) * progress;
	p2->u = (p2->u - p1.u) * progress;
	p2->v = (p2->v - p1.v) * progress;
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
		clipped[0].col = RED;
		return (1);
	}
	clipped[0] = *tri;
	clipped[1] = *tri;
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	fast_line_intersect(flags, clipped[0].p[inside_index[0]], clipped[0].p + outside_index);
	fast_line_intersect(flags, clipped[1].p[inside_index[1]], clipped[0].p + outside_index);
	clipped[1].p[inside_index[0]] = clipped[0].p[outside_index];
	clipped[0].col = LIGHT_GREY;
	clipped[1].col = YELLOW;
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