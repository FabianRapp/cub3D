/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 05:43:13 by frapp             #+#    #+#             */
/*   Updated: 2024/04/14 09:19:58 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <cub3D.h>
#include <MLX42.h>

bool	print = true;

void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

bool	out_of_bound(int p[2])
{
	int padding;

	padding = WIDTH * 0.1;
	if (p[X] < padding || p[X] >= WIDTH - padding || p[Y] < padding || p[Y] >= HEIGHT - padding)
		return (true);
	return (false);
}
/*
for
	{x1,x2,x3}
	*
	{x11,x12,x13,x14
	x21,x22,x23,x24
	x31,x32,x33,x34
	x41,x42,x43,x44}
*/
void	matrix_mult_3x1_4x4(t_vec3 *m_a, const float m_b[4][4], t_vec3 *re)
{
	float	w;

	re->p[X] = m_a->p[X] *  m_b[0][0] + m_a->p[Y] * m_b[1][0] + m_a->p[Z] * m_b[2][0] + m_b[3][0];
	re->p[Y] = m_a->p[X] *  m_b[0][1] + m_a->p[Y] * m_b[1][1] + m_a->p[Z] * m_b[2][1] + m_b[3][1];
	re->p[Z] = m_a->p[X] *  m_b[0][2] + m_a->p[Y] * m_b[1][2] + m_a->p[Z] * m_b[2][2] + m_b[3][2];
	w = m_a->p[X] *  m_b[0][3]+ m_a->p[Y] * m_b[1][3] + m_a->p[Z] * m_b[2][3] + m_b[3][3];
	if (w != 0.0f)
	{
		re->p[X] /= w;
		re->p[Y] /= w;
		re->p[Z] /= w;
	}
}


// t_vec3	matrix_mult_3x1_4x4(t_vec3 *m_a, const float m_b[4][4], t_vec3 *re)
// {
// 	t_vec3	v;
// 	float	w;

// 	re->p[X] = m_a->p[X] *  m_b[0][0] + m_a->p[Y] * m_b[1][0] + m_a->p[Z] * m_b[2][0] + m_b[3][0];
// 	re->p[Y] = m_a->p[X] *  m_b[0][1] + m_a->p[Y] * m_b[1][1] + m_a->p[Z] * m_b[2][1] + m_b[3][1];
// 	re->p[Z] = m_a->p[X] *  m_b[0][2] + m_a->p[Y] * m_b[1][2] + m_a->p[Z] * m_b[2][2] + m_b[3][2];
// 	w = m_a->p[X] *  m_b[0][3]+ m_a->p[Y] * m_b[1][3] + m_a->p[Z] * m_b[2][3] + m_b[3][3];
// 	if (w != 0.0f)
// 	{
// 		re->p[X] /= w;
// 		re->p[Y] /= w;
// 		re->p[Z] /= w;
// 	}
// 	t_vec3
// }
void	ft_put_pixel(uint8_t *pixel_buffer, int x, int y, int color)
{
	//const int	index_mult = sizeof(int32_t);

	((uint32_t *) pixel_buffer)[x + y * WIDTH] = color;
}
