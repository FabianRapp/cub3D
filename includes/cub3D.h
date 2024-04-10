/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:02:08 by fabian            #+#    #+#             */
/*   Updated: 2024/04/10 02:41:39 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*
/todo:
	- init postion
	- add basic button input to change postion


*/


# include <stdio.h>
# include <MLX42.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <math.h>
# include <limits.h>
# include <libft.h>

/*
	inlining these functions helps alot with performence!
	- saves alot performence for mixed type opperations and devison,
		when the result should be a float (1 int var 1 float var)
	-performce logs in "time_logs"

*/

#define X 0
#define Y 1
#define Z 2
#define HIGHEST_IMG_DEPTH 30	//?? somehow big numbers add a huge delay from the mlx lib
								// HIGHEST_IMG_DEPTH to HIGHEST_IMG_DEPTH-4: fps_counter
# define LOWEST_IMG_DEPTH 0


typedef int64_t	t_fixed;
#define FRACTION_BITS 32
#define F_FIXED_MULTI 4294967296
t_fixed		float_to_fixed(float nb);
t_fixed		int_to_fixed(int nb);
float		fixed_to_float(t_fixed nb);
int			fixed_to_int(t_fixed nb);
t_fixed		fixed_dev(t_fixed a, t_fixed b);
t_fixed		fixed_mult(t_fixed a, t_fixed b);
t_fixed		fixed_lerp1d(t_fixed point_a, t_fixed point_b, t_fixed progress);

#define WIDTH 1024
#define HEIGHT 1024

#define ASPECT_RATIO 2.0f / 2 // ??
//((float)HEIGHT) / ((float)WIDTH)

#define FOV 90.0f
#define X_Y_SCALAR 1 / tan(((double)FOV) / 2)
#define Z_FAR 10.0f
#define Z_NEAR 1.0f
#define Z_SCALAR (float)((((double)-Z_FAR )* Z_NEAR) / (Z_FAR - Z_NEAR))

typedef struct s_vec3
{
	float	p[3];
}	t_vec3;

typedef struct s_triangle
{
	t_vec3	p[3];
}	t_triangle;

typedef struct s_mesh
{
	t_triangle	*triangles;
	int			count;
}	t_mesh;

typedef struct s_main
{
	t_vec3	pos[3];
	t_vec3	direct[3];
	mlx_t	*mlx;
	t_mesh	cube;
}	t_main;








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
};


//utils/fps.c
struct s_fps_textures	get_fps_digit_texture(void);
void					free_fps_digit_textures(void);
void					display_fps_hook(void *param);

#endif
