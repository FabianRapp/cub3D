/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 14:46:09 by fabian            #+#    #+#             */
/*   Updated: 2024/04/05 22:30:11 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

#define WIDTH 1024
#define HEIGHT 1024
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


int32_t	main(void)
{
	if (!init())
		return (1);


	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(0, true);
	mlx_t* mlx = mlx_init(WIDTH, HEIGHT, "test", true);
	if (!mlx)
		ft_error();

	// Create and display the image.
	mlx_image_t* img = mlx_new_image(mlx, WIDTH, HEIGHT);
	if (!img || (mlx_image_to_window(mlx, img, 0, 0) < 0))
		ft_error();
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_loop_hook(mlx, ft_hook, img);
	mlx_loop_hook(mlx, display_fps_hook, mlx);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}

