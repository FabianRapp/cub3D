/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stuff.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 21:22:14 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"



// t_vec3	transform_vec3d(t_vec3 *vec3d)
// {
// 	t_vec3	transformed;

// 	transformed.p[X] = vec3d->p[X] * ((double) (FOV_RAD * ASPECT_RATIO));// / vec3d->p[Z];
// 	transformed.p[Y] = vec3d->p[Y] * ((double) FOV_RAD);// / vec3d->p[Z];
// 	transformed.p[Z] = vec3d->p[Z] * ((double) Z_NORM) + ((double) Z_OFFSET);
// 	fprintf(stderr, "base: {%f, %f, %f} transformed: {%f, %f, %f}\n", vec3d->p[X], vec3d->p[Y], vec3d->p[Z], transformed.p[X], transformed.p[Y], transformed.p[Z]);
// 	return (transformed);
// }

uint32_t	lerp_color(uint32_t max_col, double strength)
{
	uint32_t	result;

	if (strength < 0.0 || strength > 1.0)
	{
		fprintf(stderr, "error lerp color\n");
		exit (1);
	}
	result = (uint32_t)roundf(((max_col & 0xFF) * strength));
	result |= (uint32_t)roundf((((max_col & 0xFF00) >> 8) * strength)) << 8;
	result |= (uint32_t)roundf((((max_col & 0xFF0000) >> 16) * strength)) << 16;
	result |= (uint32_t)roundf((((max_col & 0xFF000000) >> 24) * strength)) << 24;
	return (result);
}

int	lerp_int(int start, int end, double pos)
{
	return (((int)((end - start) * pos)) + start);
}

// for given x range of -1 to 1
int	x_pos_x_index(double x_pos)
{
	if (x_pos > 0)
		return (lerp_int(WIDTH / 2, WIDTH, x_pos));
	else
		return (lerp_int(WIDTH / 2, 0, fabs(x_pos)));
}

// for given y ange of -1 to 1
int	y_pos_y_index(double y_pos)
{
	if (y_pos > 0)
		return(lerp_int(HEIGHT / 2, HEIGHT, y_pos));
	else
		return(lerp_int(HEIGHT / 2, 0, fabs(y_pos)));
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
				casted_pixels[x + y * img->width] = 0;
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


