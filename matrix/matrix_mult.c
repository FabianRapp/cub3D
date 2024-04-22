/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_mult.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 02:11:26 by frapp             #+#    #+#             */
/*   Updated: 2024/04/22 02:17:52 by frapp            ###   ########.fr       */
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
void	matrix_mult_vec3_4x4(t_vec3 *m_a, const float m_b[4][4], t_vec3 *re)
{
	float	w;

	re->x = m_a->x *  m_b[0][0] + m_a->y * m_b[1][0] + m_a->z * m_b[2][0] + m_b[3][0];
	re->y = m_a->x *  m_b[0][1] + m_a->y * m_b[1][1] + m_a->z * m_b[2][1] + m_b[3][1];
	re->z = m_a->x *  m_b[0][2] + m_a->y * m_b[1][2] + m_a->z * m_b[2][2] + m_b[3][2];
	w = m_a->x *  m_b[0][3]+ m_a->y * m_b[1][3] + m_a->z * m_b[2][3] + m_b[3][3];
	if (w != 0.0f)
	{
		re->x /= w;
		re->y /= w;
		re->z /= w;
	}
}

void	matrix_mult_vec3_3x3(t_vec3 v, float mb[3][3], float m_result[3])
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
