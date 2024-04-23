/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:06:43 by frapp             #+#    #+#             */
/*   Updated: 2024/04/23 23:12:12 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

void	zero_matrix(float mat[4][4])
{
	mat[0][0] = 0.0f;
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;
	mat[1][0] = 0.0f;
	mat[1][1] = 0.0f;
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;
	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = 0.0f;
	mat[2][3] = 0.0f;
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 0.0f;
}

void	projection_matrix(float mat[4][4])
{
	mat[0][0] = (float)(ASPECT_RATIO * ((float)FOV_RAD));
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;
	mat[1][0] = 0.0f;
	mat[1][1] = (float)(FOV_RAD);
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;
	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = ((float) Z_NORM);
	mat[2][3] = 1.0f;
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = ((float) Z_OFFSET);
	mat[3][3] = 0.0f;
}

void	translation_matrix(float mat[4][4], float x, float y, float z)
{
	mat[0][0] = 1.0f;
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;
	mat[1][0] = 0.0f;
	mat[1][1] = 1.0f;
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;
	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = 1.0f;
	mat[2][3] = 0.0f;
	mat[3][0] = x;
	mat[3][1] = y;
	mat[3][2] = z;
	mat[3][3] = 1.0f;
}

void	ident_mat_4x4(float mat[4][4])
{
	mat[0][0] = 1.0f;
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;
	mat[1][0] = 0.0f;
	mat[1][1] = 1.0f;
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;
	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = 1.0f;
	mat[2][3] = 0.0f;
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
}

void	rot_matx_4x4(float mat[4][4], float theta)
{
	mat[0][0] = 1.0f;
	mat[0][1] = 0.0f;
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;
	mat[1][0] = 0.0f;
	mat[1][1] = cosf(theta);
	mat[1][2] = sinf(theta);
	mat[1][3] = 0.0f;
	mat[2][0] = 0.0f;
	mat[2][1] = -sinf(theta);
	mat[2][2] = cosf(theta);
	mat[2][3] = 0.0f;
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
}

void	rot_maty_4x4(float mat[4][4], float theta)
{
	mat[0][0] = cosf(theta);
	mat[0][1] = 0.0f;
	mat[0][2] = sinf(theta);
	mat[0][3] = 0.0f;
	mat[1][0] = 0.0f;
	mat[1][1] = 1.0f;
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;
	mat[2][0] = -sinf(theta);
	mat[2][1] = 0.0f;
	mat[2][2] = cosf(theta);
	mat[2][3] = 0.0f;
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
}

void	rot_matz_4x4(float mat[4][4], float theta)
{
	mat[0][0] = cosf(theta);
	mat[0][1] = sinf(theta);
	mat[0][2] = 0.0f;
	mat[0][3] = 0.0f;
	mat[1][0] = -sinf(theta);
	mat[1][1] = cosf(theta);
	mat[1][2] = 0.0f;
	mat[1][3] = 0.0f;
	mat[2][0] = 0.0f;
	mat[2][1] = 0.0f;
	mat[2][2] = 1.0f;
	mat[2][3] = 0.0f;
	mat[3][0] = 0.0f;
	mat[3][1] = 0.0f;
	mat[3][2] = 0.0f;
	mat[3][3] = 1.0f;
}
