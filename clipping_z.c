/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_z.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/08 00:42:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

/*
	Todo:
	- refactor
	(- later: optimize the with knowledge what the plane is exactly)
*/

// d * line_direct is the line
// dot(plane_n, plane_p - any_point_on_plane) == 0 is plane
// dot(plane_p - (line_start + d * line_direct), plane_n) == 0  means line_start + d * line_direct is intersection
// sinece plane_p and line_start + d * line_direct are points on the plane this means that these two lines are parallel:
// plane_p + x * plane_n and line_start + d * line_direct + x * plane_n
// which means
// dot(plane_p, plane_n) == dot(line_start + d * line_direct, plane_n)
//==
// dot(plane_p, plane_n) == dot(line_start, plane_n) + dot(d * line_direct, plane_n)
//==
// dot(plane_p, plane_n) == dot(line_start, plane_n) + d * dot(line_direct, plane_n)
// ==
// dot(plane_p, plane_n) / dot(line_direct, plane_n) == dot(line_start, plane_n) / dot(line_direct, plane_n) + d
// ==
// (dot(plane_p, plane_n) - dot(line_start, plane_n)) / dot(line_direct, plane_n) == d
// now d * line_direct + line_start == intersection
t_vec3	line_plane_intersection(t_vec3 plane_p, t_vec3 plane_n, t_vec3 line_start, t_vec3 line_end)
{
	t_vec3	intersection;
	t_vec3	line_direct;
	float	d;
	float	dot_directline_nplane;

	line_direct = v3_sub(line_end, line_start);
	unit_vec3(&line_direct);
	dot_directline_nplane = dot_prod_unit(line_direct, plane_n);
	if (zero_f(dot_directline_nplane))
	{//todo: what to do here
		return (line_start);
	}
	d = (dot_prod_unit(plane_p, plane_n) - dot_prod_unit(line_start, plane_n)) / dot_directline_nplane;
	intersection = v3_add(v3_scale(line_direct, d), line_start);
	intersection.u = d * (line_end.u - line_start.u);
	intersection.v = d * (line_end.v - line_start.v);
	return (intersection);
}

int8_t	clipping_z_far(t_triangle *tri, t_triangle *clipped)
{
	static const t_vec3	plane_p = {.x=0, .y=0,.z = Z_FAR};
	static const t_vec3	plane_n = {.x=0, .y=0,.z = 1};
	int8_t				i;
	t_vec3				cur_start;
	int8_t				inside_points;
	int8_t				inside_index[3];

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (tri->p[i].z < Z_FAR)
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
		cur_start = tri->p[inside_index[0]];
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		clipped[0].p[outside_index1] = line_plane_intersection(plane_p, plane_n, cur_start, tri->p[outside_index1]);
		clipped[0].p[outside_index2] = line_plane_intersection(plane_p, plane_n, cur_start, tri->p[outside_index2]);
		clipped[0].col = BLUE;// TODO remove this line when clipping is fixed
		return (1);
	}
	clipped[0] = *tri;
	clipped[1] = *tri;
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	// TODO i think these triangles are wrong or atleast bad
	
	t_vec3	intersec1 = line_plane_intersection(plane_p, plane_n, tri->p[inside_index[0]], tri->p[outside_index]);
	t_vec3	intersec2 = line_plane_intersection(plane_p, plane_n, tri->p[inside_index[1]], tri->p[outside_index]);
	
	clipped[0].p[outside_index] = intersec1;

	clipped[1].p[inside_index[0]] = intersec1;
	clipped[1].p[outside_index] = intersec2;
	clipped[0].col = BLUE;// TODO remove this line when clipping is fixed
	clipped[1].col = BLUE;// TODO remove this line when clipping is fixed
	return (2);
}

int8_t	clipping_z_near(t_triangle *tri, t_triangle *clipped)
{
	static const t_vec3	plane_p = {.x=0, .y=0,.z = Z_NEAR};
	static const t_vec3	plane_n = {.x=0, .y=0,.z = 1};
	int8_t				i;
	t_vec3				cur_start;
	int8_t				inside_points;
	int8_t				inside_index[3];

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (tri->p[i].z > Z_NEAR)
			inside_index[inside_points++] = i;
		i++;
	}
	if (inside_points == 0) //inside_points == 2 ||
		return (0);
	if (inside_points == 3)
	{
		*clipped = *tri;
		return (1);
	}
	if (inside_points == 1)
	{
		clipped[0] = *tri;
		cur_start = tri->p[inside_index[0]];
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		clipped[0].p[outside_index1] = line_plane_intersection(plane_p, plane_n, cur_start, tri->p[outside_index1]);
		clipped[0].p[outside_index2] = line_plane_intersection(plane_p, plane_n, cur_start, tri->p[outside_index2]);
		print_vec3(clipped[0].p[outside_index1], "0");
		print_vec3(clipped[0].p[outside_index2], "1");
		clipped[0].col = RED;// TODO remove this line when clipping is fixed
		return (1);
	}
	clipped[0] = *tri;
	clipped[1] = *tri;
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	// TODO i think these triangles are wrong or atleast bad
	t_vec3	intersec1 = line_plane_intersection(plane_p, plane_n, tri->p[inside_index[0]], tri->p[outside_index]);
	t_vec3	intersec2 = line_plane_intersection(plane_p, plane_n, tri->p[inside_index[1]], tri->p[outside_index]);
	static int a= 0;
	clipped[0].p[outside_index] = intersec1;
	clipped[1].p[inside_index[0]] = intersec1;
	clipped[1].p[outside_index] = intersec2;
	printf("inside points: %d\n", inside_points);
	clipped[0].col = LIGHT_GREY;// TODO remove this line when clipping is fixed
	clipped[1].col = DARK_GREY;// TODO remove this line when clipping is fixed
	printf("tri 1:\n");
	print_vec3(clipped[0].p[0], 0);
	print_vec3(clipped[0].p[1], 0);
	print_vec3(clipped[0].p[2], 0);
	printf("tri 2:\n");
	print_vec3(clipped[1].p[0], 0);
	print_vec3(clipped[1].p[1], 0);
	print_vec3(clipped[1].p[2], 0);
	printf("\n");
	return (2);
}
