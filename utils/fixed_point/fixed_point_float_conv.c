/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   s_split_float.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:26:50 by fabian            #+#    #+#             */
/*   Updated: 2024/04/04 19:50:51 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>

inline t_fixed	float_to_fixed(float nb)
{
	return ((t_fixed)(nb * F_FIXED_MULTI));
}

inline t_fixed	int_to_fixed(int nb)
{
	return (((t_fixed)nb) << FRACTION_BITS);
}

inline float	fixed_to_float(t_fixed nb)
{
	return (((float)nb) / F_FIXED_MULTI);
}

inline int	fixed_to_int(t_fixed nb)
{
	return (nb >> FRACTION_BITS);
}

inline t_fixed	fixed_mult(t_fixed a, t_fixed b)
{
	return (t_fixed)((a * b) >> FRACTION_BITS);
}

inline t_fixed	fixed_dev(t_fixed a, t_fixed b)
{
	if (b)
		return ((a << FRACTION_BITS) / b);
	return (INT_MAX);
}

inline t_fixed	fixed_lerp1d(t_fixed point_a, t_fixed point_b, t_fixed progress)
{
	return (point_a + fixed_mult(point_b - point_a, progress));
}
