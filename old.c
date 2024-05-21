/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   old.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 23:23:56 by frapp             #+#    #+#             */
/*   Updated: 2024/05/21 18:01:14 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

void	scale_triangle_3d(t_triangle *tri_a, float scalar)
{
	for (int i = 0; i < 3; i++)
	{
		scale_vec3(tri_a->p + i, scalar);
	}
}

void	scale_mesh_3d(t_mesh *mesh, float scalar)
{
	for (int i = 0; i < mesh->count; i++)
	{
		scale_triangle_3d(mesh->triangles + i, scalar);
	}
}

t_triangle	apply_rotation_addtiononal_translation(t_mesh *mesh, int i)
{
	t_triangle		rotated_xz;
	t_triangle		rotated_xyz;
	t_triangle		rotated_z;
	t_triangle		translated;

	// fprintf(stderr, "p1 x: %f, y: %f z: %f\n", mesh->triangles[i].p[0].x, mesh->triangles[i].p[0].y, mesh->triangles[i].p[0].z);
	// fprintf(stderr, "p2 x: %f, y: %f z: %f\n", mesh->triangles[i].p[1].x, mesh->triangles[i].p[1].y, mesh->triangles[i].p[1].z);
	//fprintf(stderr, "p3 x: %f, y: %f z: %f\n\n", mesh->triangles[i].p[2].x, mesh->triangles[i].p[2].y, mesh->triangles[i].p[2].z);
	matrix_mult_vec3_4x4((mesh->triangles + i)->p + 0, mesh->rotation_mat_z, &rotated_z.p[0]);
	matrix_mult_vec3_4x4((mesh->triangles + i)->p + 1, mesh->rotation_mat_z, &rotated_z.p[1]);
	matrix_mult_vec3_4x4((mesh->triangles + i)->p + 2, mesh->rotation_mat_z, &rotated_z.p[2]);
	if (mesh->obj_file)
	{
		matrix_mult_vec3_4x4(&(mesh->triangles + i)->normal, mesh->rotation_mat_z, &rotated_z.normal);
		//matrix_mult_vec3_4x4((mesh->triangles + i)->obj_normal + 0, mesh->rotation_mat_z, &rotated_z.obj_normal[0]);
		//matrix_mult_vec3_4x4((mesh->triangles + i)->obj_normal + 1, mesh->rotation_mat_z, &rotated_z.obj_normal[1]);
		//matrix_mult_vec3_4x4((mesh->triangles + i)->obj_normal + 2, mesh->rotation_mat_z, &rotated_z.obj_normal[2]);
	}
	// fprintf(stderr, "p1 x: %f, y: %f z: %f\n", rotated_z.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
	// fprintf(stderr, "p2 x: %f, y: %f z: %f\n", rotated_z.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
	// fprintf(stderr, "p3 x: %f, y: %f z: %f\n\n", rotated_z.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);
	matrix_mult_vec3_4x4(rotated_z.p + 0, mesh->rotation_mat_x, &rotated_xz.p[0]);
	matrix_mult_vec3_4x4(rotated_z.p + 1, mesh->rotation_mat_x, &rotated_xz.p[1]);
	matrix_mult_vec3_4x4(rotated_z.p + 2, mesh->rotation_mat_x, &rotated_xz.p[2]);
	if (mesh->obj_file)
	{
		matrix_mult_vec3_4x4(&rotated_z.normal, mesh->rotation_mat_x, &rotated_xz.normal);
		//matrix_mult_vec3_4x4(rotated_z.obj_normal + 0, mesh->rotation_mat_x, &rotated_xz.obj_normal[0]);
		//matrix_mult_vec3_4x4(rotated_z.obj_normal + 1, mesh->rotation_mat_x, &rotated_xz.obj_normal[1]);
		//matrix_mult_vec3_4x4(rotated_z.obj_normal + 2, mesh->rotation_mat_x, &rotated_xz.obj_normal[2]);
	}
	matrix_mult_vec3_4x4(rotated_xz.p + 0, mesh->rotation_mat_y, &rotated_xyz.p[0]);
	matrix_mult_vec3_4x4(rotated_xz.p + 1, mesh->rotation_mat_y, &rotated_xyz.p[1]);
	matrix_mult_vec3_4x4(rotated_xz.p + 2, mesh->rotation_mat_y, &rotated_xyz.p[2]);
	if (mesh->obj_file)
	{
		matrix_mult_vec3_4x4(&rotated_xz.normal, mesh->rotation_mat_y, &rotated_xyz.normal);
		//matrix_mult_vec3_4x4(rotated_xz.obj_normal + 0, mesh->rotation_mat_y, &rotated_xyz.obj_normal[0]);
		//matrix_mult_vec3_4x4(rotated_xz.obj_normal + 1, mesh->rotation_mat_y, &rotated_xyz.obj_normal[1]);
		//matrix_mult_vec3_4x4(rotated_xz.obj_normal + 2, mesh->rotation_mat_y, &rotated_xyz.obj_normal[2]);
	}
	// fprintf(stderr, "p1 x: %f, y: %f z: %f\n", rotated_xz.p[0].x, rotated_z.p[0].y, rotated_z.p[0].z);
	// fprintf(stderr, "p2 x: %f, y: %f z: %f\n", rotated_xz.p[1].x, rotated_z.p[1].y, rotated_z.p[1].z);
	// fprintf(stderr, "p3 x: %f, y: %f z: %f\n\n", rotated_xz.p[2].x, rotated_z.p[2].y, rotated_z.p[2].z);
	translated = rotated_xyz;
	translated.p[0].z += 10.0f;
	translated.p[1].z += 10.0f;
	translated.p[2].z += 10.0f;



	return (translated);

}