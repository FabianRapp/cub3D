/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 05:43:13 by frapp             #+#    #+#             */
/*   Updated: 2024/04/14 05:54:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <cub3D.h>
#include <MLX42.h>


void zero_vec3(t_vec3 *v)
{
	int n = 3;

	while (n--)
	{
		v->p[n] = 0;
	}
}

void add_vec3(t_vec3 *v, t_vec3 *a)
{
	int n = 3;

	while (n--)
	{
		v->p[n] += a->p[n];
	}
}
void reverse_vec3(t_vec3 *v)
{
	int n = 3;

	while (n--)
	{
		v->p[n] *= -1;
	}
}
void multiply_vec3(t_vec3 *v, t_vec3 *a)
{
	int n = 3;

	while (n--)
	{
		v->p[n] += a->p[n];
	}
}

t_vec3 scale_vec3(t_vec3 *v, float k) 
{
	t_vec3	a;
	int		n = 3;

	while (n--)
	{
		a.p[n] = v->p[n] * k;
	}
	return a;
}

float length_vec3(t_vec3 *v)
{
	float	sum = 0;
	int		n = 3;

	while (n--)
	{
		sum += powf(v->p[n], 2);
	}
	return sqrtf(sum);
}

