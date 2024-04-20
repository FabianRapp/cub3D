
#include <cub3D.h>
#include <MLX42.h>

void	mod_cube_rotation(t_mesh *mesh, float delta_time)
{
	static float	theta = 0;

	theta += (delta_time / 4);

	#ifndef ROT_Z

	float rotation_mat_z[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#else
	float rotation_mat_z[4][4] = {
		{cosf(theta), sinf(theta), 0.0f, 0.0f},
		{-sinf(theta), cosf(theta), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	// float rotation_mat_z[4][4] = {
	// 	{cosf(theta), -sinf(theta), 0.0f, 0.0f},
	// 	{sinf(theta), cosf(theta), 0.0f, 0.0f},
	// 	{0.0f, 0.0f, 1.0f, 0.0f},
	// 	{0.0f, 0.0f, 0.0f, 1.0f},
	// };

	#endif
	ft_memcpy(mesh->rotation_mat_z, rotation_mat_z, sizeof(rotation_mat_z));
	#ifdef ROT_X
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f), 0.0f},
		{0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	#else
	float rotation_mat_x[4][4] = {
		{-1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, -1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, -1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, -1.0f}
	};
	#endif
	ft_memcpy(mesh->rotation_mat_x, rotation_mat_x, sizeof(rotation_mat_x));
	#ifndef ROT_Y
	float rotation_mat_y[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#else
	float rotation_mat_y[4][4]  = {
		{cosf(theta * 0.1f), 0.0f, sinf(theta * 0.1f), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sinf(theta * 0.1f), 0.0f, cosf(theta * 0.1f), 0.0f},
	 	{0.0f, 0.0f, 0.0f, 1.0f}
		// {1.0f, 0.0f, 0.0f, 0.0f},
		// {0.0f, 1.0f, 0.0f, 0.0f},
		// {0.0f, 0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f)},
		// {0.0f, 0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f)},
	};
	#endif
	ft_memcpy(mesh->rotation_mat_y, rotation_mat_y, sizeof(rotation_mat_y));
}

void	mod_cube_rotation2(t_mesh *mesh, float delta_time)
{
	static float	theta = 0;

	theta += delta_time;

	#ifdef ROT_Z
	float rotation_mat_z[4][4] = {
		{cosf(theta), sinf(theta), 0.0f, 0.0f},
		{-sinf(theta), cosf(theta), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	#else
	float rotation_mat_z[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#endif
	ft_memcpy(mesh->rotation_mat_z, rotation_mat_z, sizeof(rotation_mat_z));
	#ifdef ROT_X
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f), 0.0f},
		{0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};
	#else
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#endif
	ft_memcpy(mesh->rotation_mat_x, rotation_mat_x, sizeof(rotation_mat_x));
	#ifndef ROT_Y
	float rotation_mat_y[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	#else
	float rotation_mat_y[4][4]  = {
		{cosf(theta), 0.0f, sinf(theta), 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{-sinf(theta), 0.0f, cosf(theta * 0.5f), 1.0f},
	 	{0.0f, 0.0f, 0.0f, 1.0f}
		// {1.0f, 0.0f, 0.0f, 0.0f},
		// {0.0f, 1.0f, 0.0f, 0.0f},
		// {0.0f, 0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f)},
		// {0.0f, 0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f)},
	};
	#endif
	ft_memcpy(mesh->rotation_mat_y, rotation_mat_y, sizeof(rotation_mat_y));
}

void mesh_rotate_x_axis(t_mesh *mesh)
{
	static float	theta = 0;

	theta += *mesh->d_time;
	float rotation_mat_x[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, cosf(theta * 0.5f), sinf(theta * 0.5f), 0.0f},
		{0.0f, -sinf(theta * 0.5f), cosf(theta * 0.5f), 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};

	ft_memcpy(mesh->rotation_mat_x, rotation_mat_x, sizeof(rotation_mat_x));
}

void mesh_rotate_y_axis(t_mesh *mesh)
{
    static float theta = 0;

    theta += *mesh->d_time;
    float rotation_mat_y[4][4] = {
        {cosf(theta), 0.0f, -sinf(theta), 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {sinf(theta), 0.0f, cosf(theta), 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };

	ft_memcpy(mesh->rotation_mat_y, rotation_mat_y, sizeof(rotation_mat_y));
}

void mesh_rotate_z_axis(t_mesh *mesh)
{
	static float	theta = 0;

	theta += *mesh->d_time;
	float rotation_mat_z[4][4] = {
		{cosf(theta), sinf(theta), 0.0f, 0.0f},
		{-sinf(theta), cosf(theta), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f},
	};

	ft_memcpy(mesh->rotation_mat_z, rotation_mat_z, sizeof(rotation_mat_z));
}
