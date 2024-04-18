/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 05:43:13 by frapp             #+#    #+#             */
/*   Updated: 2024/04/18 06:11:45 by frapp            ###   ########.fr       */
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

t_vec3	out_of_bound(t_vec3 *v, bool print)
{
	t_vec3	bounds_result;

	zero_vec3(&bounds_result);
	if (v->x < 0)
		bounds_result.x = -1;
	else if (v->x >= WIDTH)
		bounds_result.x = 1;
	if (v->y < 0)
		bounds_result.y = -1;
	else if (v->y >= HEIGHT)
		bounds_result.y = 1;
	//printf("%f\n", v->z);
	if (v->z >= 0.98)
	{
		bounds_result.z = -1;
		//usleep(10000000);
		//exit(0);
	}
	else if (v->z >= 1)
	{
		bounds_result.z = 1;
	}
	
	return (bounds_result);
	// if (v->x < 0 || v->x >= WIDTH || v->y < 0 || v->y >= HEIGHT)
	// 	return (true);
	// if (check_z && (v->z < Z_NEAR || v->z >= Z_FAR))
	// 	return (true);
	// return (false);
}

t_vec3	out_of_bound_triangle(t_triangle *projected)
{
	t_vec3	bounds_result = out_of_bound(projected->p, projected->col = YELLOW);
	t_vec3	tmp =  out_of_bound(projected->p + 1, projected->col = YELLOW);
	add_vec3(&bounds_result, &tmp);
	tmp =  out_of_bound(projected->p + 1, projected->col = YELLOW);
	add_vec3(&bounds_result, &tmp);
	return (bounds_result);
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


// t_vec3	matrix_mult_3x1_4x4(t_vec3 *m_a, const float m_b[4][4], t_vec3 *re)
// {
// 	t_vec3	v;
// 	float	w;

// 	re->x = m_a->x *  m_b[0][0] + m_a->y * m_b[1][0] + m_a->z * m_b[2][0] + m_b[3][0];
// 	re->y = m_a->x *  m_b[0][1] + m_a->y * m_b[1][1] + m_a->z * m_b[2][1] + m_b[3][1];
// 	re->z = m_a->x *  m_b[0][2] + m_a->y * m_b[1][2] + m_a->z * m_b[2][2] + m_b[3][2];
// 	w = m_a->x *  m_b[0][3]+ m_a->y * m_b[1][3] + m_a->z * m_b[2][3] + m_b[3][3];
// 	if (w != 0.0f)
// 	{
// 		re->x /= w;
// 		re->y /= w;
// 		re->z /= w;
// 	}
// 	t_vec3
// }
void	ft_put_pixel(uint8_t *pixel_buffer, int x, int y, int color)
{
	//const int	index_mult = sizeof(int32_t);
	((uint32_t *) pixel_buffer)[x + y * WIDTH] = color;
}
