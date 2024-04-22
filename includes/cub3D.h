/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:02:08 by fabian            #+#    #+#             */
/*   Updated: 2024/04/22 02:29:57 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*
/todo:
	- init postion
	- add basic button input to change postion


*/

# include <fcntl.h>
# include <stdio.h>
# include <MLX42.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <math.h>
# include <limits.h>
# include <libft.h>
# include <time.h>
# include <float.h>

// # include <iomanip>
/*
	inlining these functions helps alot with performence!
	- saves alot performence for mixed type opperations and devison,
		when the result should be a float (1 int var 1 float var)
	-performce logs in "time_logs"

*/

#define ROT_X
//#define ROT_Y
#define ROT_Z
#define MOVEMENT

#define GRAV_CONST 0.5f

#define COLOR_COUNT 12
#define OBJ_PARSER_COLOR_COUNT 12

#define BLACK 0xFF000000
#define RED 0xFF0000FF
#define GREEN 0xFF00FF00
#define BLUE 0xFFFF0000
#define MAGENTA 0xFFFF00FF
#define YELLOW 0xFF00FFFF
#define WHITE 0xFFFFFFFF
#define CYAN 0xFF00FFFF
#define PURPLE 0xFFFF00A0
#define ORANGE 0xFF0080FF
#define TEAL 0xFF808000
#define PINK 0xFFFF80FF
#define LIME 0xFF80FF00


#define X 0
#define Y 1
#define Z 2
#define HIGHEST_IMG_DEPTH 30	//?? somehow big numbers add a huge delay from the mlx lib
								// HIGHEST_IMG_DEPTH to HIGHEST_IMG_DEPTH-4: fps_counter
# define LOWEST_IMG_DEPTH 0

typedef struct s_main	t_main;

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

#define WIDTH 1500
#define HEIGHT 1300

#define ASPECT_RATIO ((float)HEIGHT) / ((float)WIDTH)

#define FOV 60.0f
// #define FOV_RAD 1 / tan((double)FOV * 0.5 / 180.0 * 3.14159)
#define FOV_RAD 1.0 / tan(M_PI_4)
#define Z_FAR 1000.0f
#define Z_NEAR 0.1f
#define Z_NORM ((double)Z_FAR) / (Z_FAR - Z_NEAR)
#define Z_OFFSET (-(double)Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR)


#define PROJECTION_MATRIX \
{ \
	{(float)(ASPECT_RATIO * ((float)FOV_RAD)), 0.0f, 0.0f, 0.0f}, \
	{0.0f, (float)(FOV_RAD), 0.0f, 0.0f}, \
	{0.0f, 0.0f, ((float) Z_NORM), 1.0f}, \
	{0.0f, 0.0f, ((float) Z_OFFSET), 0.0f} \
}

typedef struct s_vec3
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_vec3;

#define A 3
#define R 0
#define G 1
#define B 2

typedef float t_mat_4x4[4][4];

typedef union u_color_split
{
	uint32_t	col;
	uint8_t		argb[4];
}	t_color_split;

typedef struct s_triangle
{
	t_vec3		p[3];
	float		unprojected_z[3];
	uint32_t	col;
	t_vec3		centroid;
	t_vec3		normal;
	t_vec3		obj_normal[3];
}	t_triangle;

typedef struct s_light_argb_stren
{
	float	v[4];
}	t_light_argb_stren;

typedef struct s_light
{
	t_vec3				direct;
	t_vec3				position;
	t_light_argb_stren	strength;
	t_color_split		color;
}	t_light;

typedef struct s_mesh
{
	t_triangle	*triangles;
	int			count;
	float		rotation_mat_x[4][4];
	float		rotation_mat_y[4][4];
	float		rotation_mat_z[4][4];
	t_vec3		momentum;
	t_vec3		center_pull;
	t_vec3		center;
	bool		obj_file;
	double		a;
	double		*d_time;
	mlx_image_t	*img;
	t_main		*main;
}	t_mesh;

typedef struct s_main
{
	t_vec3		camera;
	t_vec3		direct[3];
	float		world_mat[4][4];
	mlx_t		*mlx;
	t_mesh		*objs;
	int			nb;
	t_mesh		cube;
	t_mesh		cube2;
	t_mesh		tetra;
	t_mesh		skybox;
	t_mesh		custom;
	mlx_image_t	*img;
	float		depth[WIDTH * HEIGHT];
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

void	draw_mesh(t_mesh *cube_mesh);

//utils/fps.c
struct s_fps_textures	get_fps_digit_texture(void);
void					free_fps_digit_textures(void);
void					display_fps_hook(void *param);

// mesh_rotation.c
void	mod_cube_rotation(t_mesh *mesh, float delta_time);
void	mod_cube_rotation2(t_mesh *mesh, float delta_time);
mlx_image_t	*first_ob_ball(mlx_t *mlx);

void	translate_mesh_3d(t_mesh *mesh, t_vec3 v);

uint32_t	lerp_color(uint32_t max_col, float strength);

int	lerp_int(int start, int end, float pos);

// utils.c
void	ft_error(void);
t_vec3	out_of_bound(t_vec3 *v);
t_vec3	out_of_bound_triangle(t_triangle *tri);
t_vec3	out_of_bound_triangle_projeceted(t_triangle *projected);

void	ft_put_pixel(uint8_t *pixel_buffer, int x, int y, int color);
void	ft_put_pixel_fin_index(uint8_t *pixel_buffer, int index, int color);

// draw.c
void	draw_line(mlx_image_t *image, int x1, int x2, int y1, int y2, int color);
void	draw_triangle(mlx_image_t *img, t_triangle *projected, uint32_t color);
void	draw_mesh(t_mesh *mesh);
void	draw_skybox(t_mesh *mesh);

// main.c
void	ft_hook(void* param);
void	translate_triangle_3d(t_triangle *tri_a, t_vec3 v);
void	translate_mesh_3d(t_mesh *mesh, t_vec3 v);
void	scale_triangle_3d(t_triangle *tri_a, float scalar);
void	scale_mesh_3d(t_mesh *mesh, float scalar);
void	determine_centroid(t_triangle *tri);

// init_mesh.c
void	fill_tetra_mesh(t_mesh *cube, t_main *main_data);
void	fill_cube_mesh(t_mesh *cube, t_main *main_data);
void	fill_cube_mesh2(t_mesh *cube, t_main *main_data);
void	fill_skybox_mesh(t_mesh *cube, t_main *main_data);

// vec3.c
t_vec3	v3_zero(void);
t_vec3	v3_add(t_vec3 a, t_vec3 b);
t_vec3	v3_reverse(t_vec3 a);
t_vec3	v3_multiply(t_vec3 a, t_vec3 b);
t_vec3	v3_scale(t_vec3 a, float scalar);
t_vec3	v3_random(void);
t_vec3	v3_sub(t_vec3 a, t_vec3 b);
void	zero_vec3(t_vec3 *v);
void	add_vec3(t_vec3 *v, t_vec3 *a);
void	reverse_vec3(t_vec3 *v);
void	multiply_vec3(t_vec3 *v, t_vec3 *a);
void	scale_vec3(t_vec3 *v, float scalar);
float	length_vec3(t_vec3 *v);
void	print_vec3(t_vec3 v, char *msg);
t_vec3	cross_product(t_vec3 a, t_vec3 b);
float	dot_prod(t_vec3 a, t_vec3 b);
void	norm_vec3(t_vec3 *v);
void	rotate_vec3(t_vec3 *to_rotate, float first_z_rot, float x_rot, float second_z_rot);

// obj_parser.c
void	load_obj_file(char *dir, char *path, t_mesh *mesh, t_main *main_data);

//matrix/matrix_math1.c

//matrix/matrix_mult.c
void	matrix_mult_vec3_4x4(t_vec3 *m_a, const float m_b[4][4], t_vec3 *re);
void	matrix_mult_1x3_3x3(float ma[3], float mb[3][3], float m_result[3]);

//matrix/init_matrix.c
void	ident_mat_4x4(float mat[4][4]);
void	rot_matx_4x4(float mat[4][4], float theta);
void	rot_maty_4x4(float mat[4][4], float theta);
void	rot_matz_4x4(float mat[4][4], float theta);

//to_replace.c
float	generate_random_float();

#endif
