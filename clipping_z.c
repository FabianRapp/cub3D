/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_z.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

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
t_vec3	line_zplane_intersection(t_vec3 line_start, t_vec3 line_end, bool z_near)
{
	static const t_vec3	plane_far = {.x=0, .y=0,.z = Z_FAR};
	static const t_vec3	plane_n = {.x=0, .y=0,.z = 1};
	static const t_vec3	plane_near = {.x=0, .y=0,.z = Z_NEAR};
	t_vec3				plane_p;
	t_vec3	intersection;
	t_vec3	line_direct;

	if (z_near)
		plane_p = plane_near;
	else
		plane_p = plane_far;
	double	d;
	line_direct = v3_sub_incl_uv(line_end, line_start);
	if (zero_f(line_direct.z))
	{
		printf("z_near(bool): %d\n", z_near);
		print_vec3(line_start, "inside: ");
		print_vec3(line_end, "outside: ");
		print_vec3(line_direct, "line_direct: ");
	}
	assume(!zero_f(line_direct.z));
	//unit_vec3(&line_direct);
	//if (!zero_f(line_direct.z))
	//	line_direct.z = ZERO_LF * 2;
	assume(!zero_f(line_direct.z));
	if (z_near)
		d = (Z_NEAR - line_start.z) / line_direct.z;
	else
		d = (Z_FAR - line_start.z) / line_direct.z;
	//d = (dot_prod_unit(plane_p, plane_n) - dot_prod_unit(line_start, plane_n)) / line_direct.z;
	//if (!z_near)
	//	d *= -1;
	intersection = v3_add_incl_uv(v3_scale_incl_uv(line_direct, d), line_start);
	if (z_near)
	{
		if (!zero_f(intersection.z - Z_NEAR))
		{
			print_vec3(intersection, 0);
			exit(1);
		}
		intersection.z = Z_NEAR;
	}
	else
		intersection.z = Z_FAR - 1.0;
	//intersection.u = d * (line_end.u - line_start.u);
	//intersection.v = d * (line_end.v - line_start.v);
	return (intersection);
}

int8_t	clipping_z_far(t_triangle *tri, t_triangle *clipped)
{
	
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
	else if (inside_points == 3)
	{
		*clipped = *tri;
		return (1);
	}
	else if (inside_points == 1)
	{
		clipped[0] = *tri;
		cur_start = tri->p[inside_index[0]];
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		clipped[0].p[outside_index1] = line_zplane_intersection(cur_start, tri->p[outside_index1], false);
		clipped[0].p[outside_index2] = line_zplane_intersection(cur_start, tri->p[outside_index2], false);
		//if (clipped[0].p[outside_index1].z == -1.0 || clipped[0].p[outside_index2].z == -1.0)
		//	return (0);
		return (1);
	}
	clipped[0] = *tri;
	clipped[1] = *tri;
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	t_vec3	intersec1 = line_zplane_intersection(tri->p[inside_index[0]], tri->p[outside_index], false);
	t_vec3	intersec2 = line_zplane_intersection(tri->p[inside_index[1]], tri->p[outside_index], false);
	clipped[0].p[outside_index] = intersec1;
	clipped[1].p[inside_index[0]] = intersec1;
	clipped[1].p[outside_index] = intersec2;
	return (2);
}

//return the tri count afterwards
int8_t	clipping_z_near(t_triangle *tri, t_triangle *clipped)
{
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
	if (inside_points == 3)
	{
		*clipped = *tri;
		return (1);
	}
	while (inside_points == 2)
	{
		clipped[0] = *tri;
		clipped[1] = *tri;
		int8_t	outside_index = 3 - inside_index[0] - inside_index[1];
		clipped[0].p[0] = tri->p[inside_index[0]];
		clipped[0].p[1] = tri->p[inside_index[1]];
		t_vec3	debug = v3_sub(tri->p[inside_index[0]], tri->p[outside_index]);
		if (tri->p[inside_index[0]].z - tri->p[outside_index].z < ZERO_LF)
		{
			inside_points--;
			inside_index[0] = inside_index[1];
			break ;
		}
		if (tri->p[inside_index[1]].z - tri->p[outside_index].z < ZERO_LF)
		{
			inside_points--;
			break ;
		}
		clipped[0].p[2] = line_zplane_intersection(tri->p[inside_index[0]], tri->p[outside_index], true);
		clipped[1].p[0] = tri->p[inside_index[1]];
		clipped[1].p[1] = clipped[0].p[2];
		clipped[1].p[2] = line_zplane_intersection(tri->p[inside_index[1]], tri->p[outside_index], true);
		for (int i = 0; i < 3; i ++)
		{
			assume(clipped[0].p[i].z >= Z_NEAR);
			assume(clipped[1].p[i].z >= Z_NEAR);
		}
		return (2);
	}
	while (inside_points == 1)
	{
		clipped[0] = *tri;
		cur_start = tri->p[inside_index[0]];
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		if (cur_start.z - tri->p[outside_index1].z < ZERO_LF ||
			cur_start.z -  tri->p[outside_index2].z < ZERO_LF)
		{
			inside_points--;
			break ;
		}
		clipped[0].p[outside_index1] = line_zplane_intersection(cur_start, tri->p[outside_index1], true);
		clipped[0].p[outside_index2] = line_zplane_intersection(cur_start, tri->p[outside_index2], true);
		return (1);
	}
	//if (inside_points == 0)
	return (0);
}

