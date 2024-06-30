/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 05:43:13 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

bool	is_power2(uint32_t nb)
{
	return (ceil(log2(nb)) == floor(log2(nb)));
}

void	reset_camera(t_main *main_data)
{
	const t_vec3	init_cam = {.x = 0, .y = 0, .z = 0, };//.w = 1};
	const t_vec3	init_look_direct = {.x = 0, .y = 0, };//.z = 1, .w = 1};
	const t_vec3	init_up = {.x = 0.0, .y = 1, .z = 0,};// .w = 1};

	main_data->pitch = 0;
	main_data->yaw = 0;
	main_data->roll = 0;
	ft_memcpy(&main_data->world_data.camera, &init_cam, sizeof(init_cam));
	ft_memcpy(&main_data->world_data.up, &init_up, sizeof(init_up));
	unit_vec3(&main_data->world_data.up);
	ft_memcpy(&main_data->world_data.look_direct, &init_look_direct, sizeof(init_look_direct));
	unit_vec3(&main_data->world_data.look_direct);
}

void	ft_free_img(t_main *main_data, mlx_image_t **img)
{
	if (!*img)
		return ;
	mlx_delete_image(main_data->mlx, *img);
	*img = NULL;
}

void	reset_pixel_buffer(uint8_t *pixels, float *depth)
{
	uint32_t	*buffer = (uint32_t *)pixels;

	if (depth)
	{
		for (int i = 0; i < WIDTH * HEIGHT; i++)
		{
			buffer[i] = BLACK;
			depth[i] = Z_FAR;
		}
	}
	else
	{
		for (int i = 0; i < WIDTH * HEIGHT; i++)
		{
			buffer[i] = BLACK;
		}
	}
}

bool	zero_f(double f)
{
	if (fabs(f) < ZERO_LF)
	{
		return (true);
	}
	return (false);
}

void ft_error(t_main *main_data)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	cleanup_exit(main_data);
}

t_vec3	out_of_bound_projected(t_vec3 *v)
{
	t_vec3	bounds_result;

	zero_vec3(&bounds_result);
	if (v->x < 0)
		bounds_result.x = -1;
	else if (v->x >= WIDTH)
		bounds_result.x = 1;
	if (v->y < 0)
		bounds_result.y = -1;
	else if (v->y >= HEIGHT)
		bounds_result.y = 1;
	if (v->z >= 0.98)
	{
		bounds_result.z = -1;
	}
	else if (v->z >= 1)
	{
		bounds_result.z = 1;
	}
	
	return (bounds_result);

}

t_vec3	out_of_bound_triangle_projeceted(t_triangle *projected)
{
	t_vec3	bounds_result = out_of_bound_projected(projected->p);
	t_vec3	tmp =  out_of_bound_projected(projected->p + 1);
	add_vec3(&bounds_result, &tmp);
	tmp =  out_of_bound_projected(projected->p + 2);
	add_vec3(&bounds_result, &tmp);
	return (bounds_result);
}

t_vec3	out_of_bound(t_vec3 *v)
{
	t_vec3	bounds_result;

	zero_vec3(&bounds_result);
	// if (v->x < -1)
	// 	bounds_result.x = -1;
	// else if (v->x > 1)
	// 	bounds_result.x = 1;
	// if (v->y < -1)
	// 	bounds_result.y = -1;
	// else if (v->y > 1)
	// 	bounds_result.y = 1;
	// //fprintf(stderr, "%f\n", v->z);
	// if (v->z >= 0.98)
	// {
	// 	bounds_result.z = -1;
	// 	//usleep(10000000);
	// 	//exit(0);
	// }
	// else if (v->z >= 1)
	// {
	// 	bounds_result.z = 1;
	// }
	if (v->z < Z_NEAR)
		bounds_result.z = -1;
	if (v->z >= Z_FAR)
		bounds_result.z = 1;
	return (bounds_result);
}

t_vec3	out_of_bound_triangle(t_triangle *tri)
{
	t_vec3	bounds_result = out_of_bound(tri->p);
	t_vec3	tmp =  out_of_bound(tri->p + 1);
	add_vec3(&bounds_result, &tmp);
	tmp =  out_of_bound_projected(tri->p + 2);
	add_vec3(&bounds_result, &tmp);
	return (bounds_result);
}

void	ft_put_pixel(uint8_t *pixel_buffer, int x, int y, int color)
{
	//const int	index_mult = sizeof(int32_t);
	((uint32_t *) pixel_buffer)[x + y * WIDTH] = color;
}

void	ft_put_pixel_fin_index(uint8_t *pixel_buffer, int index, int color)
{
	//const int	index_mult = sizeof(int32_t);
	((uint32_t *) pixel_buffer)[index] = color;
}
