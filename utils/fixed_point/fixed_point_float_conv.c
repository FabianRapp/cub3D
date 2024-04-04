/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fixed_point_float_conv.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 18:26:50 by fabian            #+#    #+#             */
/*   Updated: 2024/04/04 19:03:48 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>

struct fixed_point_float_conv
{
	unsigned	sign:1;
	unsigned	expo:8;
	unsigned	fraction:23;
};

struct fixed_point_double_conv
{
	unsigned	sign:1;
	unsigned	expo:11;
	unsigned	fraction:52;
};

t_fixed	float_to_fixed(float nb)
{
	struct fixed_point_float_conv	*conv;

	conv = (struct fixed_point_float_conv *)&nb;
	conv->expo << 10;
	return (nb);
}

float	fixed_to_float(t_fixed nb)
{
	struct fixed_point_float_conv	*conv;
	conv = (struct fixed_point_float_conv *)&nb;
	conv->expo >> 10;
	return (nb);
}

t_fixed	dobule_to_fixed(double nb)
{
	struct fixed_point_double_conv	*conv;

	conv = (struct fixed_point_double_conv *)&nb;
	conv->expo << 10;
	return (nb);
}

float	fixed_to_double(t_fixed nb)
{
	struct fixed_point_double_conv	*conv;
	conv = (struct fixed_point_double_conv *)&nb;
	conv->expo >> 10;
	return (nb);
}

t_fixed	int_to_fixed(int nb)
{
	return (nb << 10);
}

int	fixed_to_int(t_fixed nb)
{
	return (nb >> 10);
}
