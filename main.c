/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/10 05:01:00 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

#define RED 0xFF0000FF
#define GREEN 0xFF0000FF
#define BLUE 0xFF0000FF


static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// Print the window width and height.
static void ft_hook(void* param)
{
	mlx_image_t *img = param;
	static int pixel = 0;
	static int color = RED;
	int	y = pixel / WIDTH;
	int x = pixel - (y * WIDTH);

	mlx_put_pixel(img, x, y, color);
	pixel++;
	if (pixel >= WIDTH * HEIGHT)
	{
		printf("filled !\n");
		usleep(30000000);
		pixel = 0;
		color = ~color;
		color |= 0xFF;
	}
}

bool	init(void)
{
	get_fps_digit_texture();
	return (true);
}

void	circular_mask(mlx_image_t *img)
{
	uint32_t	*casted_pixels;
	int			radius;
	int			radius_sq;
	int			x;
	int			y;
	int			center[2];
	int			y_sq;

	if (!img || !img->width || !img->height || !img->pixels)
		return ;
	if (img->width < img->height)
		radius = img->width / 2;
	else
		radius = img->height / 2;
	center[X] = img->width / 2;
	center[Y] = img->height / 2;
	y = 0;
	casted_pixels = (uint32_t*) (img->pixels);
	radius_sq = ft_powint(radius, 2);
	while (y < img->height)
	{
		x = 0;
		y_sq = ft_powint(y - center[Y], 2);
		while (x < img->width)
		{
			if (y_sq + ft_powint(x - center[X], 2) > radius_sq)
			{
				casted_pixels[x + y * img->width] &= 0x00111111; //correct for this pixel format
			}
			x++;
		}
		y++;
	}
}

mlx_image_t	*first_ob_ball(mlx_t *mlx)
{
	mlx_texture_t	*texture;
	mlx_image_t		*img;

	texture = mlx_load_png("textures/brick_wall/BricksReclaimedWhitewashedOffset001_COL_1K_METALNESS.png");
	img = mlx_texture_to_image(mlx, texture);
	mlx_resize_image(img, 400, 400);
	circular_mask(img);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_set_instance_depth(img->instances, 0);
	//img->
	return (img);
}


void	draw_triangles(t_mesh *mesh)
{
	int	tri_nb;

	tri_nb = 0;
	while (tri_nb < mesh->count)
	{

		
		tri_nb++;
	}
}

int	lerp_int(int start, int end, float pos)
{
	return (((int)((end - start) * pos)) + start);
}

// for given x range of -1 to 1
int	x_pos_x_index(float x_pos)
{
	if (x_pos > 0)
		return (lerp_int(WIDTH / 2, WIDTH, x_pos));
	else
		return (lerp_int(WIDTH / 2, 0, fabs(x_pos)));
}

// for given y ange of -1 to 1
int	y_pos_y_index(float y_pos)
{
	if (y_pos > 0)
		return(lerp_int(HEIGHT / 2, HEIGHT, y_pos));
	else
		return(lerp_int(HEIGHT / 2, 0, fabs(y_pos)));
}

void	draw_line(mlx_image_t *image, int start_pixel[2], int target_pixel[2])
{
	int	x;
	int	y;
	int	disance;
	int	counter;


	while (x != target_pixel[X] || y != target_pixel[Y])
	{
		
		mlx_put_pixel(image, x, y, RED);
	}
}

void	draw_triangle()
{
	
}

void	matrix_mult_1x3_3x3(float ma[3], float mb[3][3], float m_result[3])
{
	m_result[0] = ma[0] * mb[0][0];
	m_result[0] += ma[1] * mb[1][0];
	m_result[0] += ma[2] * mb[2][0];
	m_result[1] = ma[0] * mb[0][1];
	m_result[1] += ma[1] * mb[1][1];
	m_result[1] += ma[2] * mb[2][1];
	m_result[2] = ma[0] * mb[0][2];
	m_result[2] += ma[1] * mb[1][2];
	m_result[2] += ma[2] * mb[2][2];
}

t_vec3	transform_vec3d(t_vec3 *vec3d)
{
	t_vec3	transformed;

	transformed.p[X] = vec3d->p[X] * ((float) (X_Y_SCALAR * ASPECT_RATIO)) / vec3d->p[Z];
	transformed.p[Y] = vec3d->p[Y] * ((float) X_Y_SCALAR) / vec3d->p[Z];
	transformed.p[Z] = vec3d->p[Z] * ((float) Z_NORM) + ((float) Z_OFFSET);
	return (transformed);
}

void	fill_cube_mesh(t_mesh *cube)
{
	// t_vec3		cube_cornors[8] = {
	// 	{0, 0, 0},
	// 	{1, 0, 0},
	// 	{0, 1, 0},
	// 	{1, 1, 0},
	// 	{0, 0, 1},
	// 	{1, 0, 1},
	// 	{0, 1, 1},
	// 	{1, 1, 1},
	// };
	const t_triangle init_triangles[] = {
		// SOUTH triangles
		{{{0, 0, 0}, {0, 1, 0}, {1, 1, 0}}},
		{{{0, 0, 0}, {1, 1, 0}, {1, 0, 0}}},
		// EAST triangles
		{{{1, 0, 0}, {1, 1, 0}, {1, 1, 1}}},
		{{{1, 0, 0}, {1, 1, 1}, {1, 0, 1}}},
		// NORTH triangles
		{{{1, 0, 1}, {1, 1, 1}, {0, 1, 1}}},
		{{{1, 0, 1}, {0, 1, 1}, {0, 0, 1}}},
		// WEST triangles
		{{{0, 0, 1}, {0, 1, 1}, {0, 1, 0}}},
		{{{0, 0, 1}, {0, 1, 0}, {0, 0, 0}}},
		// TOP triangles
		{{{0, 1, 0}, {0, 1, 1}, {1, 1, 1}}},
		{{{0, 1, 0}, {1, 1, 1}, {1, 1, 0}}},
		// BOTTOM triangles
		{{{1, 0, 1}, {0, 0, 1}, {0, 0, 0}}},
		{{{1, 0, 1}, {0, 0, 0}, {1, 0, 0}}}
	};
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
}

void	draw_cube(t_mesh *cube_mesh)
{
	int			i;
	t_triangle	*cur_tri;
	t_triangle	projected;

	i = 0;
	while (i < cube_mesh->count)
	{
		cur_tri = cube_mesh->triangles + i;
		projected.p[0] = transform_vec3d(cur_tri->p + 0);
		projected.p[1] = transform_vec3d(cur_tri->p + 1);
		projected.p[2] = transform_vec3d(cur_tri->p + 2);
		//draw
		i++;
	}
}

int32_t	main(void)
{
	mlx_image_t	*ob;
	t_main		m_data;



	fill_cube_mesh(&m_data.cube);
	draw_cube(&m_data.cube);

	if (!init())
		return (1);
	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(0, true);
	m_data.mlx = mlx_init(WIDTH, HEIGHT, "test", true);
	if (!m_data.mlx)
		ft_error();

	// Create and display the image.
	mlx_image_t* img = mlx_new_image(m_data.mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(m_data.mlx, img, 0, 0) < 0))
		ft_error();
	mlx_set_instance_depth(img->instances, 1);
	first_ob_ball(m_data.mlx);
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_loop_hook(m_data.mlx, ft_hook, img);
	mlx_loop_hook(m_data.mlx, display_fps_hook, m_data.mlx);
	mlx_loop(m_data.mlx);
	mlx_terminate(m_data.mlx);
	return (EXIT_SUCCESS);
}

