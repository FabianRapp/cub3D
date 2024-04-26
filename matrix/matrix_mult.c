/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_mult.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:11:26 by frapp             #+#    #+#             */
/*   Updated: 2024/04/26 11:37:37 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

/*
for
	{x1,x2,x3}
	*
	{x11,x12,x13,x14
	x21,x22,x23,x24
	x31,x32,x33,x34
	x41,x42,x43,x44}
*/

void	mat4x4_mult_mat4x4(float ma[4][4], float mb[4][4], float result[4][4])
{
	result[0][0] = ma[0][0] * mb[0][0] + ma[0][1] * mb[1][0] + ma[0][2] * mb[2][0] + ma[0][3] * mb[3][0];
	result[0][1] = ma[0][0] * mb[0][1] + ma[0][1] * mb[1][1] + ma[0][2] * mb[2][1] + ma[0][3] * mb[3][1];
	result[0][2] = ma[0][0] * mb[0][2] + ma[0][1] * mb[1][2] + ma[0][2] * mb[2][2] + ma[0][3] * mb[3][2];
	result[0][3] = ma[0][0] * mb[0][3] + ma[0][1] * mb[1][3] + ma[0][2] * mb[2][3] + ma[0][3] * mb[3][3];
	result[1][0] = ma[1][0] * mb[0][0] + ma[1][1] * mb[1][0] + ma[1][2] * mb[2][0] + ma[1][3] * mb[3][0];
	result[1][1] = ma[1][0] * mb[0][1] + ma[1][1] * mb[1][1] + ma[1][2] * mb[2][1] + ma[1][3] * mb[3][1];
	result[1][2] = ma[1][0] * mb[0][2] + ma[1][1] * mb[1][2] + ma[1][2] * mb[2][2] + ma[1][3] * mb[3][2];
	result[1][3] = ma[1][0] * mb[0][3] + ma[1][1] * mb[1][3] + ma[1][2] * mb[2][3] + ma[1][3] * mb[3][3];
	result[2][0] = ma[2][0] * mb[0][0] + ma[2][1] * mb[1][0] + ma[2][2] * mb[2][0] + ma[2][3] * mb[3][0];
	result[2][1] = ma[2][0] * mb[0][1] + ma[2][1] * mb[1][1] + ma[2][2] * mb[2][1] + ma[2][3] * mb[3][1];
	result[2][2] = ma[2][0] * mb[0][2] + ma[2][1] * mb[1][2] + ma[2][2] * mb[2][2] + ma[2][3] * mb[3][2];
	result[2][3] = ma[2][0] * mb[0][3] + ma[2][1] * mb[1][3] + ma[2][2] * mb[2][3] + ma[2][3] * mb[3][3];
	result[3][0] = ma[3][0] * mb[0][0] + ma[3][1] * mb[1][0] + ma[3][2] * mb[2][0] + ma[3][3] * mb[3][0];
	result[3][1] = ma[3][0] * mb[0][1] + ma[3][1] * mb[1][1] + ma[3][2] * mb[2][1] + ma[3][3] * mb[3][1];
	result[3][2] = ma[3][0] * mb[0][2] + ma[3][1] * mb[1][2] + ma[3][2] * mb[2][2] + ma[3][3] * mb[3][2];
	result[3][3] = ma[3][0] * mb[0][3] + ma[3][1] * mb[1][3] + ma[3][2] * mb[2][3] + ma[3][3] * mb[3][3];
}

// void	matrix_mult_vec3_4x4(t_vec3 *v, const float m_b[4][4], t_vec3 *re)
// {
// 	re->x = v->x *  m_b[0][0] + v->y * m_b[1][0] + v->z * m_b[2][0] + m_b[3][0];
// 	re->y = v->x *  m_b[0][1] + v->y * m_b[1][1] + v->z * m_b[2][1] + m_b[3][1];
// 	re->z = v->x *  m_b[0][2] + v->y * m_b[1][2] + v->z * m_b[2][2] + m_b[3][2];
// 	re->w = v->x *  m_b[0][3]+ v->y * m_b[1][3] + v->z * m_b[2][3] + m_b[3][3];
// 	if (re->w != 0.0f)
// 	{
// 		re->x /= re->w;
// 		re->y /= re->w;
// 		re->z /= re->w;
// 	}
// }

void	matrix_mult_vec3_4x4(t_vec3 *v, const float m_b[4][4], t_vec3 *re)
{
	re->x = v->x *  m_b[0][0] + v->y * m_b[1][0] + v->z * m_b[2][0] + v->w * m_b[3][0];
	re->y = v->x *  m_b[0][1] + v->y * m_b[1][1] + v->z * m_b[2][1] + v->w * m_b[3][1];
	re->z = v->x *  m_b[0][2] + v->y * m_b[1][2] + v->z * m_b[2][2] + v->w * m_b[3][2];
	re->w = v->x *  m_b[0][3]+ v->y * m_b[1][3] + v->z * m_b[2][3] + v->w * m_b[3][3];
}

void	matrix_mult_1x3_3x3(t_vec3 v, float mb[3][3], float m_result[3])
{
	m_result[0] = v.x * mb[0][0];
	m_result[0] += v.y * mb[1][0];
	m_result[0] += v.z * mb[2][0];
	m_result[1] = v.x * mb[0][1];
	m_result[1] += v.y * mb[1][1];
	m_result[1] += v.z * mb[2][1];
	m_result[2] = v.x * mb[0][2];
	m_result[2] += v.y * mb[1][2];
	m_result[2] += v.z * mb[2][2];
}


