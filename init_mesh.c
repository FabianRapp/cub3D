/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mesh.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:07:42 by frapp             #+#    #+#             */
/*   Updated: 2024/04/20 01:08:51 by frapp            ###   ########.fr       */
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
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {0, 0, 1}, {dx, 0, dy}}, YELLOW, 1},
		//LEFT
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {dx, 0, dy}, {dy, 0, dx}}, YELLOW, 1},
		{.p={{M_SQRT1_2, 1.0f, M_SQRT1_2}, {dy, 0, dx}, {dx, 0, dy}}, YELLOW, 1},
		// BOTTOM triangles
		{.p={{0, 0, 1}, {dx, 0, dy}, {dy, 0, dx}}, YELLOW, 1},
	};
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	t_vec3 momentum = {0.3, -1, 0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->main = main_data;
	cube->obj_file = false;
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
	t_vec3 momentum = {1, 0, 0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	cube->main = main_data;
	cube->obj_file = false;
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
	float rotation_mat[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	ft_memcpy(mesh->rotation_mat_x, rotation_mat, sizeof(rotation_mat));
	ft_memcpy(mesh->rotation_mat_y, rotation_mat, sizeof(rotation_mat));
	ft_memcpy(mesh->rotation_mat_z, rotation_mat, sizeof(rotation_mat));
	t_vec3 momentum = {0, 0, 0};
	ft_memcpy(&mesh->momentum, &momentum, sizeof(momentum));
	mesh->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	mesh->count = sizeof(init_triangles) / sizeof(t_triangle);
	mesh->main = main_data;
	mesh->d_time = &main_data->mlx->delta_time;
	mesh->img = main_data->img;
	mesh->obj_file = false;
}


void	fill_cube_mesh2(t_mesh *cube, t_main *main_data)
{
	const t_triangle init_triangles[] = {
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
	};

	// const t_triangle init_triangles[] = {
	// 	{.p={{0, 0, 1}, {0, 1, 1}, {1, 1, 1}}, .col=RED},
	// 	{.p={{0, 0, 1}, {1, 1, 1}, {1, 0, 1}}, .col=GREEN},
	// 	// EAST triangles
	// 	{.p={{1, 0, 1}, {1, 1, 1}, {1, 1, 2}}, .col=BLUE},
	// 	{.p={{1, 0, 1}, {1, 1, 2}, {1, 0, 2}}, .col=MAGENTA},
	// 	// NORTH triangles
	// 	{.p={{1, 0, 2}, {1, 1, 2}, {0, 1, 2}}, .col=YELLOW},
	// 	{.p={{1, 0, 2}, {0, 1, 2}, {0, 0, 2}}, .col=WHITE},
	// 	// WEST triangles
	// 	{.p={{0, 0, 2}, {0, 1, 2}, {0, 1, 1}}, .col=CYAN},
	// 	{.p={{0, 0, 2}, {0, 1, 1}, {0, 0, 1}}, .col=PURPLE},
	// 	// TOP triangles
	// 	{.p={{0, 1, 1}, {0, 1, 2}, {1, 1, 2}}, .col=TEAL},
	// 	{.p={{0, 1, 1}, {1, 1, 2}, {1, 1, 1}}, .col=PINK},
	// 	// BOTTOM triangles
	// 	{.p={{1, 0, 2}, {0, 0, 2}, {0, 0, 1}}, .col=ORANGE},
	// 	{.p={{1, 0, 2}, {0, 0, 1}, {1, 0, 1}}, .col=LIME},
	// };
	t_vec3 momentum = {0, 1, 0};
	ft_memcpy(&cube->momentum, &momentum, sizeof(momentum));
	cube->triangles = ft_memdup(&init_triangles, sizeof(init_triangles));
	cube->count = sizeof(init_triangles) / sizeof(t_triangle);
	cube->main = main_data;
	cube->obj_file = false;
}
