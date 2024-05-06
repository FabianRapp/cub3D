/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 05:43:13 by frapp             #+#    #+#             */
/*   Updated: 2024/05/06 08:58:30 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include <cub3D.h>
#include <MLX42.h>

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

bool	zero_f(float f)
{
	if (fabs(f) < 0.00001)
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
