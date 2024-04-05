/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fps.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabian <fabian@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:30:36 by fabian            #+#    #+#             */
/*   Updated: 2024/04/05 23:49:16 by fabian           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>

void	print_instance(mlx_instance_t *instance)
{
	printf("--------------Instance---------------\n");
	if (!instance)
	{
		printf("no instance!!\n");
		return ;
	}
	printf("x: %d\n", instance->x);
	printf("y: %d\n", instance->y);
	printf("z: %d\n", instance->z);
	if (instance->enabled)
		printf("enabled\n");
	else
		printf("disabled\n");
	printf("-------------------------------------\n");
}

// needs to be called once during init and then when ever needed
struct s_fps_textures	get_fps_digit_texture(void)
{
	static struct s_fps_textures	fps_digits_textures = {0};

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
}

mlx_image_t	*display_digit(mlx_t *mlx, int digit, mlx_instance_t instance)
{

	static struct s_fps_textures	digits = {0};
	mlx_image_t						*digit_im;

	if (!digits.zero)
		digits = get_fps_digit_texture();
	if (digit == 0)
		digit_im = mlx_texture_to_image(mlx, digits.zero);
	else if (digit == 1)
		digit_im = mlx_texture_to_image(mlx, digits.one);
	else if (digit == 2)
		digit_im = mlx_texture_to_image(mlx, digits.two);
	else if (digit == 3)
		digit_im = mlx_texture_to_image(mlx, digits.three);
	else if (digit == 4)
		digit_im = mlx_texture_to_image(mlx, digits.four);
	else if (digit == 5)
		digit_im = mlx_texture_to_image(mlx, digits.five);
	else if (digit == 6)
		digit_im = mlx_texture_to_image(mlx, digits.six);
	else if (digit == 7)
		digit_im = mlx_texture_to_image(mlx, digits.seven);
	else if (digit == 8)
		digit_im = mlx_texture_to_image(mlx, digits.eight);
	else if (digit == 9)
		digit_im = mlx_texture_to_image(mlx, digits.nine);
	mlx_image_to_window(mlx, digit_im, instance.x, instance.y);
	mlx_set_instance_depth(digit_im->instances, instance.z);
	// free(digit_im->instances);
	// digit_im->instances = NULL;
	// digit_im->instances = ft_memdup(&instance, sizeof(instance));
	// *(digit_im->instances) = instance;
	return (digit_im);
}

int	ft_u_nb_len(unsigned nb)
{
	int count;

	if (!nb)
		return (1);
	count = 0;
	while (nb)
	{
		nb /= 10;
		count++;
	}
	return (count);
}

void	display_fps(mlx_t *mlx, int fps, mlx_instance_t instance, int i)
{
	int								digit_count;
	int								digit_width;
	int								digit_hight;
	static mlx_image_t				*digit_imges[1024] = {NULL};

	if (!fps || i >= 1024)
		return ;
	digit_count = ft_u_nb_len(fps);
	digit_width = mlx->width / digit_count / 10;
	digit_hight = mlx->height / digit_count / 10;
	if (!i)
	{
		while (digit_imges[i])
		{
			mlx_delete_image(mlx, digit_imges[i]);
			digit_imges[i] = NULL;
			i++;
		}
		i = 0;
		// printf("digit width: %d\ndigit height: %d\n", digit_width, digit_hight);
		instance.x = mlx->width - (digit_width * digit_count);//digit_width;
		instance.y = digit_hight / 2;//digit_hight;
		instance.z = HIGHEST_IMG_DEPTH;
	}
	//print_instance(&instance);

	digit_imges[i++] = display_digit(mlx, fps % 10, instance);
	instance.x -= digit_width;
	instance.z--;
	display_fps(mlx, fps / 10, instance, i);
}

void	display_fps_hook(void *param)
{
	static double				sum_delta_time = 0.001f;
	static int16_t				frames = 0;
	mlx_t						*mlx;
	int							fps;
	static const mlx_instance_t	instance = {-1, -1, -1, false};

	mlx = param;
	sum_delta_time += mlx->delta_time;
	frames++;
	if (sum_delta_time > 0.3)
	{
		fps = (int)(frames / sum_delta_time);
		display_fps(mlx, fps, instance, 0);
		printf("%d\n", fps);
		sum_delta_time = 0.001f;
		frames = 0;
	}


}
