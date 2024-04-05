/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:02:08 by fabian            #+#    #+#             */
/*   Updated: 2024/04/05 20:49:43 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <stdio.h>
# include <MLX42.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <math.h>
# include <limits.h>

/*
	inlining these functions helps alot with performence!
	- saves alot performence for mixed type opperations and devison,
		when the result should be a float (1 int var 1 float var)
	-performce logs in "time_logs"

*/
typedef int32_t	t_fixed;

#define FRACTION_BITS 10
#define F_FIXED_MULTI 1024

t_fixed		float_to_fixed(float nb);
t_fixed		int_to_fixed(int nb);
float		fixed_to_float(t_fixed nb);
int			fixed_to_int(t_fixed nb);
t_fixed		fixed_dev(t_fixed a, t_fixed b);
t_fixed		fixed_mult(t_fixed a, t_fixed b);
t_fixed		fixed_2dlerp(t_fixed point_a, t_fixed point_b, t_fixed progress);

struct s_fps_textures
{
	mlx_texture_t	*zero;
	mlx_texture_t	*one;
	mlx_texture_t	*two;
	mlx_texture_t	*three;
	mlx_texture_t	*four;
	mlx_texture_t	*five;
	mlx_texture_t	*six;
	mlx_texture_t	*seven;
	mlx_texture_t	*eight;
	mlx_texture_t	*nine;
	mlx_texture_t	*dot;
};


//utils/fps.c
struct s_fps_textures	get_fps_digit_texture(void);
void					free_fps_digit_textures(void);
void					display_fps_hook(void *param);

#endif
