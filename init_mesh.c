/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mesh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:07:42 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

void	fill_tetra_mesh(t_mesh *cube, t_main *main_data)
{
	double dx = 0.96;//cosf(M_PI_4 / 3.0);
	double dy = 0.25;//sinf(M_PI_4 / 3.0);
	const t_triangle init_triangles[] = {
		//front
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {0, 0, 1}, {dx, 0, dy}}, .col=YELLOW, 1},
		//LEFT
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {dx, 0, dy}, {dy, 0, dx}}, .col=YELLOW, 1},
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {dy, 0, dx}, {dx, 0, dy}}, .col=YELLOW, 1},
		// BOTTOM triangles
		{.p={{0, 0, 1}, {dx, 0, dy}, {dy, 0, dx}}, YELLOW, 1},
	};
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	cube->main = main_data;
	cube->obj_file = false;
	for (int i = 0; i < cube->count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cube->triangles[i].p[j].mtl = NULL;
		}
	}
}


void	fill_cube_mesh(t_mesh *cube, t_main *main_data)
{
	u_int32_t color = RED;
	const t_triangle init_triangles[] = {
		// SOUTH triangles
		{.p={{0, 0, -1}, {0, 1, -1}, {1, 1, -1}}, .col=RED},
		{.p={{0, 0, -1}, {1, 1, -1}, {1, 0, -1}}, .col=RED},
		// EAST triangles
		{.p={{1, 0, -1}, {1, 1, -1}, {1, 1, -2}}, .col=RED},
		{.p={{1, 0, -1}, {1, 1, -2}, {1, 0, -2}}, .col=RED},
		// NORTH triangles
		{.p={{1, 0, -2}, {1, 1, -2}, {0, 1, -2}}, .col=RED},
		{.p={{1, 0, -2}, {0, 1, -2}, {0, 0, -2}}, .col=RED},
		// WEST triangles
		{.p={{0, 0, -2}, {0, 1, -2}, {0, 1, -1}}, .col=RED},
		{.p={{0, 0, -2}, {0, 1, -1}, {0, 0, -1}}, .col=RED},
		// TOP triangles
		{.p={{0, 1, -1}, {0, 1, -2}, {1, 1, -2}}, .col=RED},
		{.p={{0, 1, -1}, {1, 1, -2}, {1, 1, -1}}, .col=RED},
		// BOTTOM triangles
		{.p={{1, 0, -2}, {0, 0, -2}, {0, 0, -1}}, .col=RED},
		{.p={{1, 0, -2}, {0, 0, -1}, {1, 0, -1}}, .col=RED},
	};
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	cube->main = main_data;
	cube->obj_file = false;
	for (int i = 0; i < cube->count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cube->triangles[i].p[j].mtl = NULL;
		}
	}
}


void	fill_skybox_mesh(t_mesh *mesh, t_main *main_data)
{
	u_int32_t color = RED;
	const t_triangle init_triangles[] = {
		{.p={{-1, -1, 0}, {-1, 1, 0}, {1, 1, 0}}, .col=RED},
		{.p={{-1, -1, 0}, {1, 1, 0}, {1, -1, 0}}, .col=GREEN},
		// EAST triangles
		{.p={{1, -1, 0}, {1, 1, 0}, {1, 1, Z_FAR}}, .col=BLUE},
		{.p={{1, -1, 0}, {1, 1, Z_FAR}, {1, -1, Z_FAR}}, .col=MAGENTA},
		// NORTH triangles
		{.p={{1, -1, Z_FAR}, {1, 1, Z_FAR}, {-1, 1, Z_FAR}}, .col=YELLOW},
		{.p={{1, -1, Z_FAR}, {-1, 1, Z_FAR}, {-1, -1, Z_FAR}}, .col=WHITE},
		// WEST triangles
		{.p={{-1, -1, Z_FAR}, {-1, 1, Z_FAR}, {-1, 1, 0}}, .col=CYAN},
		{.p={{-1, -1, Z_FAR}, {-1, 1, 0}, {-1, -1, 0}}, .col=PURPLE},
		// TOP triangles
		{.p={{-1, 1, 0}, {-1, 1, Z_FAR}, {1, 1, Z_FAR}}, .col=TEAL},
		{.p={{-1, 1, 0}, {1, 1, Z_FAR}, {1, 1, 0}}, .col=PINK},
		// BOTTOM triangles
		{.p={{1, -1, Z_FAR}, {-1, -1, Z_FAR}, {-1, -1, 0}}, .col=ORANGE},
		{.p={{1, -1, Z_FAR}, {-1, -1, 0}, {1, -1, 0}}, .col=LIME},
	};
	mesh->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	mesh->count = sizeof(init_triangles) / sizeof(t_triangle);
	mesh->main = main_data;
	mesh->img = main_data->img;
	mesh->obj_file = false;
	for (int i = 0; i < mesh->count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			mesh->triangles[i].p[j].mtl = NULL;
		}
	}
}


void	fill_cube_mesh2(t_mesh *cube, t_main *main_data)
{
	const t_triangle init_triangles[] = {
		{.p={{0, 0, 0, .w = 1}, {0, 1, 0, .w = 1}, {1, 1, 0, .w = 1}}, .col=RED},
		{.p={{0, 0, 0, .w = 1}, {1, 1, 0, .w = 1}, {1, 0, 0, .w = 1}}, .col=GREEN},
		// EAST triangles
		{.p={{1, 0, 0, .w = 1}, {1, 1, 0, .w = 1}, {1, 1, 1, .w = 1}}, .col=BLUE},
		{.p={{1, 0, 0, .w = 1}, {1, 1, 1, .w = 1}, {1, 0, 1, .w = 1}}, .col=MAGENTA},
		// NORTH triangles
		{.p={{1, 0, 1, .w = 1}, {1, 1, 1, .w = 1}, {0, 1, 1, .w = 1}}, .col=YELLOW},
		{.p={{1, 0, 1, .w = 1}, {0, 1, 1, .w = 1}, {0, 0, 1, .w = 1}}, .col=WHITE},
		// WEST triangles
		{.p={{0, 0, 1, .w = 1}, {0, 1, 1, .w = 1}, {0, 1, 0, .w = 1}}, .col=CYAN},
		{.p={{0, 0, 1, .w = 1}, {0, 1, 0, .w = 1}, {0, 0, 0, .w = 1}}, .col=PURPLE},
		// TOP triangles
		{.p={{0, 1, 0, .w = 1}, {0, 1, 1, .w = 1}, {1, 1, 1, .w = 1}}, .col=TEAL},
		{.p={{0, 1, 0, .w = 1}, {1, 1, 1, .w = 1}, {1, 1, 0, .w = 1}}, .col=PINK},
		// BOTTOM triangles
		{.p={{1, 0, 1, .w = 1}, {0, 0, 1, .w = 1}, {0, 0, 0, .w = 1}}, .col=ORANGE},
		{.p={{1, 0, 1, .w = 1}, {0, 0, 0, .w = 1}, {1, 0, 0, .w = 1}}, .col=LIME},
	};
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	cube->main = main_data;
	cube->obj_file = false;
	for (int i = 0; i < cube->count; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cube->triangles[i].p[j].mtl = NULL;
		}
	}
}
