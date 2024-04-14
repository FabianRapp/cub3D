/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:02:08 by fabian            #+#    #+#             */
/*   Updated: 2024/04/13 21:23:04 by frapp            ###   ########.fr       */
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

#define ASPECT_RATIO ((float)HEIGHT) / ((float)WIDTH)

#define FOV 90.0f
#define FOV_RAD 1 / tan((double)FOV * 0.5 / 180.0 * 3.14159)
#define Z_FAR 1000.0f
#define Z_NEAR 0.1f
#define Z_NORM ((double)Z_FAR) / (Z_FAR - Z_NEAR)
#define Z_OFFSET (-(double)Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR)

// not no functions supporting this yet
#define PROJECTION_MATRIX \
{ \
	{(float)(ASPECT_RATIO * ((float)FOV_RAD)), 0.0f, 0.0f, 0.0f}, \
	{0.0f, (float)(FOV_RAD), 0.0f, 0.0f}, \
	{0.0f, 0.0f, ((float) Z_NORM), 1.0f}, \
	{0.0f, 0.0f, ((float) Z_OFFSET), 0.0f} \
}


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
	float		rotation_mat_x[4][4];
	float		rotation_mat_z[4][4];
	mlx_image_t	*img;
}	t_mesh;

typedef struct s_main
{
	t_vec3		pos[3];
	t_vec3		direct[3];
	mlx_t		*mlx;
	t_mesh		cube;
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

void	draw_cube(t_mesh *cube_mesh);

//utils/fps.c
struct s_fps_textures	get_fps_digit_texture(void);
void					free_fps_digit_textures(void);
void					display_fps_hook(void *param);




mlx_image_t	*first_ob_ball(mlx_t *mlx);
#endif
