/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:02:08 by fabian            #+#    #+#             */
/*   Updated: 2024/06/18 01:01:11 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

/*
/todo:
	- it seems the unprojected z val i sometimes wrong leading to triangles beeing drawn partly eventhough they are behind others,
		might be diffrent reason aswell
	- clipping against Z axis seems to not work correctly (shows when triangle clips 2 other sides for z < Z_NEAR)

*/

# include <fcntl.h>
# include <stdio.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <math.h>
# include <limits.h>
# include "libft.h"
# include <time.h>

// out headers
# include "menu.h"

#define unreachable __builtin_unreachable

//#define NDEBUG 1
#if defined(NDEBUG)
#define assume(cond) do { if (!(cond)) __builtin_unreachable(); } while (0)
#else
#include <assert.h>
#define assume(cond) assert(cond)
#endif

#define ROT_X
//#define ROT_Y
#define ROT_Z
//#define MOVEMENT

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
#define LIGHT_GREY 0xFFD3D3D3
#define DARK_GREY 0xFFA9A9A9
//=== DEFAULT KEY BINDS======

#define FORWARD_KEY MLX_KEY_W
#define LEFT_KEY MLX_KEY_A
#define RIGHT_KEY MLX_KEY_D
#define BACKWARDS_KEY MLX_KEY_S

#define JUMP_KEY MLX_KEY_SPACE
#define NEGATIVE_JUMP_KEY MLX_KEY_Z

#define PAUSE_KEY MLX_KEY_P
#define MENU_KEY MLX_KEY_M
#define CURSORE_MODE_TOGGLE MLX_KEY_T

//=============================

#define MOUSE_SENS_BASE (0.001f)

#define X 0
#define Y 1
#define Z 2

#define FPS_DEPTH 4
#define MENU_DEPTH 3
#define MAIN_RENDER_DEPTH 2

typedef struct s_menu	t_menu;
typedef struct s_main	t_main;

typedef int64_t	t_index_usage;

typedef int64_t	t_fixed;
#define FRACTION_BITS 32
#define F_FIXED_MULTI 4294967296
t_fixed		double_to_fixed(double nb);
t_fixed		int_to_fixed(int nb);
double		fixed_to_double(t_fixed nb);
int			fixed_to_int(t_fixed nb);
t_fixed		fixed_dev(t_fixed a, t_fixed b);
t_fixed		fixed_mult(t_fixed a, t_fixed b);
t_fixed		fixed_lerp1d(t_fixed point_a, t_fixed point_b, t_fixed progress);


// todo should be enum
#define MOUSE_SENS_WIDGET_INDEX 0

#define WIDTH (256 * 10)
#define HEIGHT (256 * 8)

#define ASPECT_RATIO ((double)HEIGHT) / ((double)WIDTH)

#define FOV 60.0f
#define FOV_RAD 1 / tan((double)FOV * 0.5 / 180.0 * 3.14159)
//#define FOV_RAD 1.0471975512 // precomputed for FOV = 60.0 degrees
//#define FOV_RAD 1.73205257663 // precomputed for FOV = 60.0 degrees
//#define FOV_RAD 1.0 / tan(M_PI_4)
#define Z_FAR 1000.0f
#define Z_NEAR 0.1f
#define Z_NORM ((double)Z_FAR) / (Z_FAR - Z_NEAR)
#define Z_OFFSET (-(double)Z_FAR * Z_NEAR) / (Z_FAR - Z_NEAR)

#define PROJECTION_MATRIX \
{ \
	{(double)(ASPECT_RATIO * ((double)FOV_RAD)), 0.0f, 0.0f, 0.0f}, \
	{0.0f, (double)(FOV_RAD), 0.0f, 0.0f}, \
	{0.0f, 0.0f, ((double) Z_NORM), 1.0f}, \
	{0.0f, 0.0f, ((double) Z_OFFSET), 0.0f} \
}

typedef struct s_mtl
{
	char			*lib_name;
	char			*name;
	double			ns;
	double			ka[3];
	double			ks[3];
	double			ke[3];
	double			ni;
	double			d;
	double			illum;
	char			*map_kd;
	mlx_texture_t	*texture;
	int				index;
}	t_mtl;



typedef struct s_vec3
{
	double			x;//HAS TO BE FIRST
	double			y;//HAS TO BE SECOND
	double			z;//HAS TO BE THIRD
	double			w;
	t_mtl			*mtl; // HAS TO BE AFTER POSITION DATA
	double			u;
	double			v;
	double			unprojected_z;
}	t_vec3;

typedef struct s_triangle
{
	t_vec3		p[3];// HAS TO BE FIRST
	//double		unprojected_z[3];// HAS TO BE SECOND
	uint32_t	col; //HAS TO BE THIRD
	t_vec3		centroid;
	t_vec3		normal;
	t_vec3		obj_normal[3]; 
}	t_triangle;

#define A 3
#define R 0
#define G 1
#define B 2

typedef double t_mat_4x4[4][4];

typedef union u_color_split
{
	uint32_t	col;
	uint8_t		argb[4];
}	t_color_split;

typedef struct s_light_argb_stren
{
	double	v[4];
}	t_light_argb_stren;

typedef struct s_light
{
	t_vec3				direct;
	t_vec3				position;
	t_light_argb_stren	strength;
	t_color_split		color;
}	t_light;

typedef struct s_physics_data
{
	double			*delta_time;
	double			x_speed;
	double			y_speed;
	double			z_speed;
}	t_physics_data;

typedef struct s_model_space_data
{
	double			model_matrix[4][4];
	double			x_scale;
	double			y_scale;
	double			z_scale;
	double			x_translation;
	double			y_translation;
	double			z_translation;
	double			x_rotation;
	double			y_rotation;
	double			z_rotation;
	t_vec3			center;
}	t_model_space_data;

typedef struct s_world_data
{
	t_vec3	camera;
	t_vec3	look_direct;
	t_vec3	up;
}	t_world_data;

typedef struct s_mesh
{
	t_triangle			*triangles;
	int					count;
	t_model_space_data	model_space_data;
	t_world_data		*world_data;
	t_physics_data		physics_data;
	bool				obj_file;
	mlx_image_t			*img;
	t_main				*main;
	t_mtl				**mtl_libs;
	int					mtl_count;
}	t_mesh;

typedef struct	s_movement_state
{
	uint8_t	up:1;
	uint8_t	back:1;
	uint8_t	left:1;
	uint8_t	right:1;
	uint8_t	jump:1;
	uint8_t	negative_jump:1;
}	t_movement_state;

typedef struct s_controls
{
	//t_vec3				movement_speed;
	double				movement_speed_straight;
	double				movement_speed_left;
	double				movement_speed_right;
	double				movement_speed_back;
	t_movement_state	state;
	double				jump_height;
}	t_controls;

typedef struct s_settings
{
	uint8_t	cursor_lock:1;
	uint8_t	cursor_hide:1;
	uint8_t	disable_cursor:1;
	uint8_t	paused:1;
	
	double	mouse_sens;
}	t_settings;

typedef struct s_main
{
	_Alignas(128) double depth[WIDTH * HEIGHT];
	double		yaw;
	double		pitch;
	double		roll;
	mlx_t		*mlx;
	t_mesh		*objs;
	int			nb;
	t_mesh		*meshes;
	t_world_data	world_data;
	int			mesh_count;
	mlx_image_t	*img;
	t_controls	controls;
	t_settings	settings;
	t_menu		menu;
}	t_main;

// todo: get better textures
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

// main.c

//old.c

//utils/fps.c
struct s_fps_textures	get_fps_digit_texture(void);
void					free_fps_digit_textures(void);
void					display_fps_hook(void *param);

mlx_image_t	*first_ob_ball(mlx_t *mlx);

void	translate_mesh_3d(t_mesh *mesh, t_vec3 v);

uint32_t	lerp_color(uint32_t max_col, double strength);

int	lerp_int(int start, int end, double pos);

// utils.c
void	ft_error(t_main *main_data);
t_vec3	out_of_bound(t_vec3 *v);
t_vec3	out_of_bound_triangle(t_triangle *tri);
t_vec3	out_of_bound_triangle_projeceted(t_triangle *projected);
bool	zero_f(double f);
void	reset_pixel_buffer(uint8_t *pixels, double *depth);
void	ft_free_img(t_main *main_data, mlx_image_t **img);

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
t_vec3	v3_scale(t_vec3 a, double scalar);
t_vec3	v3_sub(t_vec3 a, t_vec3 b);
void	zero_vec3(t_vec3 *v);
void	add_vec3(t_vec3 *v, t_vec3 *a);
void	reverse_vec3(t_vec3 *v);
void	multiply_vec3(t_vec3 *v, t_vec3 *a);
void	scale_vec3(t_vec3 *v, double scalar);
double	length_vec3(t_vec3 *v);
void	print_vec3(t_vec3 v, char *msg);
t_vec3	cross_product(t_vec3 a, t_vec3 b);
double	dot_prod_unit(t_vec3 a, t_vec3 b);
void	unit_vec3(t_vec3 *v);
void	rotate_vec3(t_vec3 *to_rotate, double x_rot, double first_z_rot, double second_z_rot);
void	div_vec3(t_vec3 *v, double a);
void	init_vec3(t_vec3 *v, double x, double y, double z);
t_vec3	vec3_init(double x, double y, double z);
t_vec3	get_direction(double pitch, double yaw, double roll);

// obj_parser.c
t_mesh	load_obj_file(char *dir, char *path, t_main *main_data);

//matrix/matrix_math1.c

//matrix/matrix_mult.c
void	matrix_mult_vec3_4x4(t_vec3 *v, const double m_b[4][4], t_vec3 *re);
void	matrix_mult_1x3_3x3(t_vec3 v, double mb[3][3], double m_result[3]);
void	mat4x4_mult_mat4x4(double ma[4][4], double mb[4][4], double result[4][4]);

//matrix/init_matrix.c
void	ident_mat_4x4(double mat[4][4]);
void	scale_matrix(double mat[4][4], double x, double y, double z);
void	rot_matx_4x4(double mat[4][4], double theta);
void	rot_maty_4x4(double mat[4][4], double theta);
void	rot_matz_4x4(double mat[4][4], double theta);
void	translation_matrix(double mat[4][4], double x, double y, double z);
void	projection_matrix(double mat[4][4]);
void	zero_matrix(double mat[4][4]);
void	matrix_point_at(t_vec3 *pos, t_vec3 *target, t_vec3 *up, double result[4][4]);
void	matrix_look_at(double point_at_mat[4][4], double look_at_mat[4][4]);

//matrix/debug.c
void	print_matrix(double m[4][4]);

// fill_triangle.c
double	slope_2d_x_per_y(t_vec3 p1, t_vec3 p2);
void	sort_vertexes_for_y(t_triangle *tri);
void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_scalars);

// fill_triangle2.c
void	fill_triangle_color(mlx_image_t *img, t_triangle *projected, uint32_t color, t_mesh *mesh);

// clipping_z.c
int8_t	clipping_z_near(t_triangle *tri, t_triangle *clipped);
int8_t	clipping_z_far(t_triangle *tri, t_triangle *clipped);

//clipping_xy.c
int8_t		call_clipping_xy(t_triangle *clipped);

// key_handlers.c
void	ft_key_hook(mlx_key_data_t keydata, void *param);
void	wasd_key_handler(mlx_key_data_t keydata, void *param);
void	jump_key_handler(mlx_key_data_t keydata, void *param);
void	settings_key_handler(mlx_key_data_t keydata, t_main *main_data);
void	key_hook(mlx_key_data_t keydata, void *param);
void	cursor_hook(double xpos, double ypos, void* param);
void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);


// init.c
void	init_main(t_main *main_data);
void	init_hooks(t_main *main_data);
void	init_menu_widgets(t_main *main_data);
void	init_default_model_space(t_model_space_data *data);
void	init_basic_data_mesh(t_main *main_data, t_mesh *mesh);
void	make_mesh_render_rdy(t_mesh *mesh);


// utils/cleanup.c
void	cleanup_exit(void *m_data);
void	clean_mlx(t_main *main_data);
void	free_mesh(t_mesh *mesh);

// clipping_z.c
int8_t	clipping_z_near(t_triangle *tri, t_triangle *clipped);
int8_t	clipping_z_far(t_triangle *tri, t_triangle *clipped);

//clipping_xy.c
int8_t		call_clipping_xy(t_triangle *clipped);

// key_handlers.c
void	ft_key_hook(mlx_key_data_t keydata, void *param);
void	wasd_key_handler(mlx_key_data_t keydata, void *param);
void	jump_key_handler(mlx_key_data_t keydata, void *param);
void	settings_key_handler(mlx_key_data_t keydata, t_main *main_data);
void	key_hook(mlx_key_data_t keydata, void *param);
void	cursor_hook(double xpos, double ypos, void* param);
void	mouse_hook(mouse_key_t button, action_t action, modifier_key_t mods, void *param);


// init.c
void	init_main(t_main *main_data);
void	init_hooks(t_main *main_data);
void	init_menu_widgets(t_main *main_data);

// utils/cleanup.c
void	cleanup_exit(void *m_data);
void	clean_mlx(t_main *main_data);
void	free_mesh(t_mesh *mesh);

#endif



