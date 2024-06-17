/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:06:43 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../MLX42/include/MLX42/MLX42.h"

void	zero_matrix(double mat[4][4])
{
	mat[0][0] = 0.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = 0.0;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 0.0;
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 0.0;
}

void	scale_matrix(double mat[4][4], double x, double y, double z)
{
	mat[0][0] = x;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = y;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = z;
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;
}

void	projection_matrix(double mat[4][4])
{
	mat[0][0] = (double)(ASPECT_RATIO * ((double)FOV_RAD));
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = (double)(FOV_RAD);
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = ((double) Z_NORM);
	mat[2][3] = 1.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = ((double) Z_OFFSET);
	mat[3][3] = 0.0;
}

void	translation_matrix(double mat[4][4], double x, double y, double z)
{
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = 0.0;
	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;
	mat[3][3] = 1.0;
}

void	ident_mat_4x4(double mat[4][4])
{
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;
}

void	rot_matx_4x4(double mat[4][4], double theta)
{
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = cos(theta);
	mat[1][2] = sin(theta);
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = -sin(theta);
	mat[2][2] = cos(theta);
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;
}

void	rot_maty_4x4(double mat[4][4], double theta)
{
	mat[0][0] = cos(theta);
	mat[0][1] = 0.0;
	mat[0][2] = sin(theta);
	mat[0][3] = 0.0;
	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = -sin(theta);
	mat[2][1] = 0.0;
	mat[2][2] = cos(theta);
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;
}

void	rot_matz_4x4(double mat[4][4], double theta)
{
	mat[0][0] = cos(theta);
	mat[0][1] = sin(theta);
	mat[0][2] = 0.0;
	mat[0][3] = 0.0;
	mat[1][0] = -sin(theta);
	mat[1][1] = cos(theta);
	mat[1][2] = 0.0;
	mat[1][3] = 0.0;
	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
	mat[2][3] = 0.0;
	mat[3][0] = 0.0;
	mat[3][1] = 0.0;
	mat[3][2] = 0.0;
	mat[3][3] = 1.0;
}

//acomment_imgs/rotating_space_2d.png
//acomment_imgs/point_at_mat.png
void	matrix_point_at(t_vec3 *pos, t_vec3 *target, t_vec3 *up, double result[4][4])
{
	t_vec3	new_forward;
	t_vec3	new_up;
	t_vec3	a;
	t_vec3	new_right;

	new_forward = v3_sub(*target, *pos);// get the look direct
	unit_vec3(&new_forward);

	a = v3_scale(new_forward, dot_prod_unit(*up, new_forward));

	new_up = v3_sub(*up, a); //can not look straight up
	unit_vec3(&new_up);
	//print_vec3(new_up, "new_up.");

	new_right = cross_product(new_up, new_forward);
	//new_right = cross_product(new_forward, new_up);

	//why this order not [0]: forward, [1]:right, [2]: up?
	result[0][0] = new_right.x;
	result[0][1] = new_right.y;
	result[0][2] = new_right.z;
	result[0][3] = 0.0;
	result[1][0] = new_up.x;
	result[1][1] = new_up.y;
	result[1][2] = new_up.z;
	result[1][3] = 0.0;
	result[2][0] = new_forward.x;
	result[2][1] = new_forward.y;
	result[2][2] = new_forward.z;
	result[2][3] = 0.0;
	result[3][0] = pos->x;
	result[3][1] = pos->y;
	result[3][2] = pos->z;
	result[3][3] = 1.0;
}

//iverse for point_at_mat (does not inverse any matrix)
//acomment_imgs/rotating_space_2d.png
void	matrix_look_at(double point_at_mat[4][4], double look_at_mat[4][4])
{
	look_at_mat[0][0] = point_at_mat[0][0];
	look_at_mat[0][1] = point_at_mat[1][0];
	look_at_mat[0][2] = point_at_mat[2][0];
	look_at_mat[0][3] = 0.0;
	look_at_mat[1][0] = point_at_mat[0][1];
	look_at_mat[1][1] = point_at_mat[1][1];
	look_at_mat[1][2] = point_at_mat[2][1];
	look_at_mat[1][3] = 0.0;
	look_at_mat[2][0] = point_at_mat[0][2];
	look_at_mat[2][1] = point_at_mat[1][2];
	look_at_mat[2][2] = point_at_mat[2][2];
	look_at_mat[2][3] = 0.0;
	look_at_mat[3][0] = -(point_at_mat[3][0] * look_at_mat[0][0] + point_at_mat[3][1] * look_at_mat[1][0] + point_at_mat[3][2] * look_at_mat[2][0]);
	look_at_mat[3][1] = -(point_at_mat[3][0] * look_at_mat[0][1] + point_at_mat[3][1] * look_at_mat[1][1] + point_at_mat[3][2] * look_at_mat[2][1]);
	look_at_mat[3][2] = -(point_at_mat[3][0] * look_at_mat[0][2] + point_at_mat[3][1] * look_at_mat[1][2] + point_at_mat[3][2] * look_at_mat[2][2]);
	// look_at_mat[3][0] = -(point_at_mat[0][0] * point_at_mat[3][0] + point_at_mat[0][1] * point_at_mat[3][1] + point_at_mat[0][2] * point_at_mat[3][2]);//- dot prod of point_at[0], point_at[3]
	// look_at_mat[3][1] = -(point_at_mat[1][0] * point_at_mat[3][0] + point_at_mat[1][1] * point_at_mat[3][1] + point_at_mat[1][2] * point_at_mat[3][2]);
	// look_at_mat[3][2] = -(point_at_mat[2][0] * point_at_mat[3][0] + point_at_mat[2][1] * point_at_mat[3][1] + point_at_mat[2][2] * point_at_mat[3][2]);
	look_at_mat[3][3] = 1.0;
}
