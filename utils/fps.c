/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:30:36 by fabian            #+#    #+#             */
/*   Updated: 2024/04/04 19:01:22 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>

// needs to be called once during init and then when ever needed
struct s_fps_textures	get_fps_digit_texture(void)
{
	static struct s_fps_textures	fps_digits_textures = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	if (fps_digits_textures.zero)
		return (fps_digits_textures);
	fps_digits_textures.zero = mlx_load_png("pngs/fps_counter/0.png");
	fps_digits_textures.one = mlx_load_png("pngs/fps_counter/1.png");
	fps_digits_textures.two = mlx_load_png("pngs/fps_counter/2.png");
	fps_digits_textures.three = mlx_load_png("pngs/fps_counter/3.png");
	fps_digits_textures.four = mlx_load_png("pngs/fps_counter/4.png");
	fps_digits_textures.five = mlx_load_png("pngs/fps_counter/5.png");
	fps_digits_textures.six = mlx_load_png("pngs/fps_counter/6.png");
	fps_digits_textures.seven = mlx_load_png("pngs/fps_counter/7.png");
	fps_digits_textures.eight = mlx_load_png("pngs/fps_counter/8.png");
	fps_digits_textures.nine = mlx_load_png("pngs/fps_counter/9.png");
	fps_digits_textures.dot = mlx_load_png("pngs/fps_counter/dot.png");
	return (fps_digits_textures);
}

// called before exit to cleanup
// its ok to call this before the init
// the mlx loop should not run anymore when this is called
void	free_fps_digit_textures(void)
{
	struct s_fps_textures	fps_digits_textures;

	fps_digits_textures = get_fps_digit_texture();
	if (fps_digits_textures.zero)
		mlx_delete_texture(fps_digits_textures.zero);
	if (fps_digits_textures.one)
		mlx_delete_texture(fps_digits_textures.one);
	if (fps_digits_textures.two)
		mlx_delete_texture(fps_digits_textures.two);
	if (fps_digits_textures.three)
		mlx_delete_texture(fps_digits_textures.three);
	if (fps_digits_textures.four)
		mlx_delete_texture(fps_digits_textures.four);
	if (fps_digits_textures.five)
		mlx_delete_texture(fps_digits_textures.five);
	if (fps_digits_textures.six)
		mlx_delete_texture(fps_digits_textures.six);
	if (fps_digits_textures.seven)
		mlx_delete_texture(fps_digits_textures.seven);
	if (fps_digits_textures.eight)
		mlx_delete_texture(fps_digits_textures.eight);
	if (fps_digits_textures.nine)
		mlx_delete_texture(fps_digits_textures.nine);
	if (fps_digits_textures.dot)
		mlx_delete_texture(fps_digits_textures.dot);
}

void	display_fps_hook(void *param)
{
	static double	sum_delta_time = 0.001f;
	static int16_t	frames = 0;
	mlx_t			*mlx;

	mlx = param;
	sum_delta_time += mlx->delta_time;
	frames++;
	if (sum_delta_time > 1)
	{
		printf("fps: %.2f\n", frames / sum_delta_time);
		sum_delta_time = 0.001f;
		frames = 0;
	}

	struct s_fps_textures digits = get_fps_digit_texture();
	mlx_image_t *digit_im = mlx_texture_to_image(mlx, digits.one);
	if (!digit_im || (mlx_image_to_window(mlx, digit_im, 0, 0) < 0))
	{
		//error
	}
}
