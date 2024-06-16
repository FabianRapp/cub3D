/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_split_double.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:26:50 by fabian            #+#    #+#             */
/*   Updated: 2024/04/04 19:50:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/cub3D.h"

t_fixed	double_to_fixed(double nb)
{
	return ((t_fixed)(nb * F_FIXED_MULTI));
}

t_fixed	int_to_fixed(int nb)
{
	return (((t_fixed)nb) << FRACTION_BITS);
}

double	fixed_to_double(t_fixed nb)
{
	return (((double)nb) / F_FIXED_MULTI);
}

int	fixed_to_int(t_fixed nb)
{
	return (nb >> FRACTION_BITS);
}

t_fixed	fixed_mult(t_fixed a, t_fixed b)
{
	return (t_fixed)((a * b) >> FRACTION_BITS);
}

t_fixed	fixed_dev(t_fixed a, t_fixed b)
{
	if (b)
		return ((a << FRACTION_BITS) / b);
	return (INT_MAX);
}

t_fixed	fixed_lerp1d(t_fixed point_a, t_fixed point_b, t_fixed progress)
{
	return (point_a + fixed_mult(point_b - point_a, progress));
}
