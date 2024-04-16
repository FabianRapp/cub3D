/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mesh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:07:42 by frapp             #+#    #+#             */
/*   Updated: 2024/04/17 01:00:18 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

void	fill_tetra_mesh(t_mesh *cube, t_main *main_data)
{
	float dx = 0.96;//cosf(M_PI_4 / 3.0);
	float dy = 0.25;//sinf(M_PI_4 / 3.0);
	const t_triangle init_triangles[] = {
		//front
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {0, 0, 0}, {dx, 0, dy}}, YELLOW, 0},
		//LEFT
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {dx, 0, dy}, {dy, 0, dx}}, YELLOW, 0},
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {dy, 0, dx}, {dx, 0, dy}}, YELLOW, 0},
		// BOTTOM triangles
		{.p={{0, 0, 0}, {dx, 0, dy}, {dy, 0, dx}}, YELLOW, 0},
	};
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	t_vec3 momentum = {0.3, -1, 0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->main = main_data;
}


void	fill_cube_mesh(t_mesh *cube, t_main *main_data)
{
	u_int32_t color = RED;
	const t_triangle init_triangles[] = {
		// SOUTH triangles
		{.p={{0, 0, 0}, {0, 1, 0}, {1, 1, 0}}, .col=RED},
		{.p={{0, 0, 0}, {1, 1, 0}, {1, 0, 0}}, .col=GREEN},
		// EAST triangles
		{.p={{1, 0, 0}, {1, 1, 0}, {1, 1, 1}}, .col=BLUE},
		{.p={{1, 0, 0}, {1, 1, 1}, {1, 0, 1}}, .col=MAGENTA},
		// NORTH triangles
		{.p={{1, 0, 1}, {1, 1, 1}, {0, 1, 1}}, .col=YELLOW},
		{.p={{1, 0, 1}, {0, 1, 1}, {0, 0, 1}}, .col=WHITE},
		// WEST triangles
		{.p={{0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, .col=CYAN},
		{.p={{0, 0, 1}, {0, 1, 0}, {0, 0, 0}}, .col=PURPLE},
		// TOP triangles
		{.p={{0, 1, 0}, {0, 1, 1}, {1, 1, 1}}, .col=TEAL},
		{.p={{0, 1, 0}, {1, 1, 1}, {1, 1, 0}}, .col=PINK},
		// BOTTOM triangles
		{.p={{1, 0, 1}, {0, 0, 1}, {0, 0, 0}}, .col=ORANGE},
		{.p={{1, 0, 1}, {0, 0, 0}, {1, 0, 0}}, .col=LIME},
		// {.p={{0, 0, 0}, {0, 1, 0}, {1, 1, 0}}, .col=RED},
		// {.p={{0, 0, 0}, {1, 1, 0}, {1, 0, 0}}, .col=RED},
		// // EAST triangles
		// {.p={{1, 0, 0}, {1, 1, 0}, {1, 1, 1}}, .col=RED},
		// {.p={{1, 0, 0}, {1, 1, 1}, {1, 0, 1}}, .col=RED},
		// // NORTH triangles
		// {.p={{1, 0, 1}, {1, 1, 1}, {0, 1, 1}}, .col=RED},
		// {.p={{1, 0, 1}, {0, 1, 1}, {0, 0, 1}}, .col=RED},
		// // WEST triangles
		// {.p={{0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, .col=RED},
		// {.p={{0, 0, 1}, {0, 1, 0}, {0, 0, 0}}, .col=RED},
		// // TOP triangles
		// {.p={{0, 1, 0}, {0, 1, 1}, {1, 1, 1}}, .col=RED},
		// {.p={{0, 1, 0}, {1, 1, 1}, {1, 1, 0}}, .col=RED},
		// // BOTTOM triangles
		// {.p={{1, 0, 1}, {0, 0, 1}, {0, 0, 0}}, .col=RED},
		// {.p={{1, 0, 1}, {0, 0, 0}, {1, 0, 0}}, .col=RED},

		// // SOUTH triangles
		// {.p={{0, 1, 0}, {1, 1, 0}, {1, 0, 0}}, .col=RED},
		// {.p={{0, 0, 0}, {0, 0, 0}, {1, 1, 0}}, .col=RED},
		// // EAST triangles
		// {.p={{1, 1, 0}, {1, 1, 1}, {1, 0, 1}}, .col=RED},
		// {.p={{1, 0, 0}, {1, 0, 0}, {1, 1, 1}}, .col=RED},
		// // NORTH triangles
		// {.p={{1, 1, 1}, {0, 1, 1}, {0, 0, 1}}, .col=RED},
		// {.p={{1, 0, 1}, {1, 0, 1}, {0, 1, 1}}, .col=RED},
		// // WEST triangles
		// {.p={{0, 1, 1}, {0, 1, 0}, {0, 0, 0}}, .col=RED},
		// {.p={{0, 0, 1}, {0, 0, 1}, {0, 1, 0}}, .col=RED},
		// // TOP triangles
		// {.p={{0, 1, 1}, {1, 1, 1}, {1, 1, 0}}, .col=RED},
		// {.p={{0, 1, 0}, {0, 1, 0}, {1, 1, 1}}, .col=RED},
		// // BOTTOM triangles
		// {.p={{0, 0, 1}, {0, 0, 0}, {1, 0, 0}}, .col=RED},
		// {.p={{1, 0, 1}, {1, 0, 1}, {0, 0, 0}}, .col=RED},
		
		// // SOUTH triangles
		// {.p={{1, 1, 0}, {0, 0, 0}, {0, 1, 0}}, .col=RED},
		// {.p={{1, 0, 0}, {0, 0, 0}, {1, 1, 0}}, .col=RED},
		// // EAST triangles
		// {.p={{1, 1, 1}, {1, 0, 0}, {1, 1, 0}}, .col=RED},
		// {.p={{1, 0, 1}, {1, 0, 0}, {1, 1, 1}}, .col=RED},
		// // NORTH triangles
		// {.p={{0, 1, 1}, {1, 0, 1}, {1, 1, 1}}, .col=RED},
		// {.p={{0, 0, 1}, {1, 0, 1}, {0, 1, 1}}, .col=RED},
		// // WEST triangles
		// {.p={{0, 1, 0}, {0, 0, 1}, {0, 1, 1}}, .col=RED},
		// {.p={{0, 0, 0}, {0, 0, 1}, {0, 1, 0}}, .col=RED},
		// // TOP triangles
		// {.p={{1, 1, 1}, {0, 1, 0}, {0, 1, 1}}, .col=RED},
		// {.p={{1, 1, 0}, {0, 1, 0}, {1, 1, 1}}, .col=RED},
		// // BOTTOM triangles
		// {.p={{0, 0, 0}, {1, 0, 1}, {0, 0, 1}}, .col=RED},
		// {.p={{1, 0, 0}, {1, 0, 1}, {0, 0, 0}}, .col=RED},

		// TOP triangles
		// {.p={{0, 2, 0}, {0, 2, 1}, {1, 1, 1}}, .col=RED},
		// {.p={{2, 2, 0}, {2, 2, 1}, {1, 1, 0}}, .col=RED},

	};
	t_vec3 momentum = {1.0, 0.5, 100};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	// const t_vec3 offset = {0, 0, 0.25};
	// translate_mesh_3d(cube, offset);
	// scale_mesh_3d(cube, 0.25);
	cube->main = main_data;
}

void	fill_cube_mesh2(t_mesh *cube, t_main *main_data)
{
	const t_triangle init_triangles[] = {
		// SOUTH triangles
		{.p={{0.75, 0.75, 0.25}, {0.25, 0.75, 0.25}, {0.25, 0.25, 0.25}}, .col=GREEN},
		{.p={{0.75, 0.25, 0.25}, {0.25, 0.25, 0.25}, {0.75, 0.75, 0.25}}, .col=GREEN},
		// EAST triangles
		{.p={{0.75, 0.75, 0.75}, {0.75, 0.75, 0.25}, {0.75, 0.25, 0.25}}, .col=GREEN},
		{.p={{0.75, 0.25, 0.75}, {0.75, 0.25, 0.25}, {0.75, 0.75, 0.75}}, .col=GREEN},
		// NORTH triangles
		{.p={{0.25, 0.75, 0.75}, {0.75, 0.75, 0.75}, {0.75, 0.25, 0.75}}, .col=GREEN},
		{.p={{0.25, 0.25, 0.75}, {0.75, 0.25, 0.75}, {0.25, 0.75, 0.75}}, .col=GREEN},
		// WEST triangles
		{.p={{0.25, 0.75, 0.25}, {0.25, 0.75, 0.75}, {0.25, 0.25, 0.75}}, .col=GREEN},
		{.p={{0.25, 0.25, 0.25}, {0.25, 0.25, 0.75}, {0.25, 0.75, 0.25}}, .col=GREEN},
		// TOP triangles
		{.p={{0.75, 0.75, 0.75}, {0.25, 0.75, 0.75}, {0.25, 0.75, 0.25}}, .col=GREEN},
		{.p={{0.75, 0.75, 0.25}, {0.25, 0.75, 0.25}, {0.75, 0.75, 0.75}}, .col=GREEN},
		// BOTTOM triangles
		{.p={{0.25, 0.25, 0.25}, {0.25, 0.25, 0.75}, {0.75, 0.25, 0.75}}, .col=GREEN},
		{.p={{0.75, 0.25, 0.25}, {0.75, 0.25, 0.75}, {0.25, 0.25, 0.25}}, .col=GREEN},
	};
	t_vec3 momentum = {0.5, -0.25, 0};
	// static double	theta = 0;
	// theta += *cube->d_time * 0.000000001;
	//t_vec3 momentum = {cos(theta), sin(theta), 0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	cube->main = main_data;
}
