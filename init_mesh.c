/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mesh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:07:42 by frapp             #+#    #+#             */
/*   Updated: 2024/04/16 14:17:27 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

void	fill_tetra_mesh(t_mesh *cube)
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
}


void	fill_cube_mesh(t_mesh *cube)
{
	u_int32_t color = RED;
	const t_triangle init_triangles[] = {
		// SOUTH triangles
		{.p={{0, 0, 0}, {0, 1, 0}, {1, 1, 0}}, .col=RED},
		{.p={{0, 0, 0}, {1, 1, 0}, {1, 0, 0}}, .col=RED},
		// EAST triangles
		{.p={{1, 0, 0}, {1, 1, 0}, {1, 1, 1}}, .col=RED},
		{.p={{1, 0, 0}, {1, 1, 1}, {1, 0, 1}}, .col=RED},
		// NORTH triangles
		{.p={{1, 0, 1}, {1, 1, 1}, {0, 1, 1}}, .col=RED},
		{.p={{1, 0, 1}, {0, 1, 1}, {0, 0, 1}}, .col=RED},
		// WEST triangles
		{.p={{0, 0, 1}, {0, 1, 1}, {0, 1, 0}}, .col=RED},
		{.p={{0, 0, 1}, {0, 1, 0}, {0, 0, 0}}, .col=RED},
		// TOP triangles
		{.p={{0, 1, 0}, {0, 1, 1}, {1, 1, 1}}, .col=RED},
		{.p={{0, 1, 0}, {1, 1, 1}, {1, 1, 0}}, .col=RED},
		// BOTTOM triangles
		{.p={{1, 0, 1}, {0, 0, 1}, {0, 0, 0}}, .col=RED},
		{.p={{1, 0, 1}, {0, 0, 0}, {1, 0, 0}}, .col=RED},

		// SOUTH triangles
		{.p={{0, 1, 0}, {1, 1, 0}, {1, 0, 0}}, .col=RED},
		{.p={{0, 0, 0}, {0, 0, 0}, {1, 1, 0}}, .col=RED},
		// EAST triangles
		{.p={{1, 1, 0}, {1, 1, 1}, {1, 0, 1}}, .col=RED},
		{.p={{1, 0, 0}, {1, 0, 0}, {1, 1, 1}}, .col=RED},
		// NORTH triangles
		{.p={{1, 1, 1}, {0, 1, 1}, {0, 0, 1}}, .col=RED},
		{.p={{1, 0, 1}, {1, 0, 1}, {0, 1, 1}}, .col=RED},
		// WEST triangles
		{.p={{0, 1, 1}, {0, 1, 0}, {0, 0, 0}}, .col=RED},
		{.p={{0, 0, 1}, {0, 0, 1}, {0, 1, 0}}, .col=RED},
		// TOP triangles
		{.p={{0, 1, 1}, {1, 1, 1}, {1, 1, 0}}, .col=RED},
		{.p={{0, 1, 0}, {0, 1, 0}, {1, 1, 1}}, .col=RED},
		// BOTTOM triangles
		{.p={{0, 0, 1}, {0, 0, 0}, {1, 0, 0}}, .col=RED},
		{.p={{1, 0, 1}, {1, 0, 1}, {0, 0, 0}}, .col=RED},
		
		// SOUTH triangles
		{.p={{1, 1, 0}, {0, 0, 0}, {0, 1, 0}}, .col=RED},
		{.p={{1, 0, 0}, {0, 0, 0}, {1, 1, 0}}, .col=RED},
		// EAST triangles
		{.p={{1, 1, 1}, {1, 0, 0}, {1, 1, 0}}, .col=RED},
		{.p={{1, 0, 1}, {1, 0, 0}, {1, 1, 1}}, .col=RED},
		// NORTH triangles
		{.p={{0, 1, 1}, {1, 0, 1}, {1, 1, 1}}, .col=RED},
		{.p={{0, 0, 1}, {1, 0, 1}, {0, 1, 1}}, .col=RED},
		// WEST triangles
		{.p={{0, 1, 0}, {0, 0, 1}, {0, 1, 1}}, .col=RED},
		{.p={{0, 0, 0}, {0, 0, 1}, {0, 1, 0}}, .col=RED},
		// TOP triangles
		{.p={{1, 1, 1}, {0, 1, 0}, {0, 1, 1}}, .col=RED},
		{.p={{1, 1, 0}, {0, 1, 0}, {1, 1, 1}}, .col=RED},
		// BOTTOM triangles
		{.p={{0, 0, 0}, {1, 0, 1}, {0, 0, 1}}, .col=RED},
		{.p={{1, 0, 0}, {1, 0, 1}, {0, 0, 0}}, .col=RED},

		// TOP triangles
		// {.p={{0, 2, 0}, {0, 2, 1}, {1, 1, 1}}},
		// {.p={{2, 2, 0}, {2, 2, 1}, {1, 1, 0}}},

	};
	t_vec3 momentum = {0.0, 0.5, 0.0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
}

void	fill_cube_mesh2(t_mesh *cube)
{
	const t_triangle init_triangles[] = {
		// SOUTH triangles
		{.p={{1/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}}, .col=GREEN},
		{.p={{1/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}, {1/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}}, .col=GREEN},
		// EAST triangles
		{.p={{1/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}, {1/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}, {1/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}}, .col=GREEN},
		{.p={{1/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}, {1/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}, {1/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}}, .col=GREEN},
		// NORTH triangles
		{.p={{0/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}, {1/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}, {1/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}}, .col=GREEN},
		{.p={{0/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}, {1/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}}, .col=GREEN},
		// WEST triangles
		{.p={{0/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}, {0/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}}, .col=GREEN},
		{.p={{0/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}}, .col=GREEN},
		// TOP triangles
		{.p={{1/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}}, .col=GREEN},
		{.p={{1/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 1/2.0+0.25, 0/2.0+0.25}, {1/2.0+0.25, 1/2.0+0.25, 1/2.0+0.25}}, .col=GREEN},
		// BOTTOM triangles
		{.p={{0/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}, {0/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}, {1/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}}, .col=GREEN},
		{.p={{1/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}, {1/2.0+0.25, 0/2.0+0.25, 1/2.0+0.25}, {0/2.0+0.25, 0/2.0+0.25, 0/2.0+0.25}}, .col=GREEN},

	};
	t_vec3 momentum = {0.5, 0, 0};
	// static double	theta = 0;
	// theta += *cube->d_time * 0.000000001;
	//t_vec3 momentum = {cos(theta), sin(theta), 0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
}
