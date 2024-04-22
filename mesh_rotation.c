
#include <cub3D.h>
#include <MLX42.h>

void	mod_cube_rotation(t_mesh *mesh, float delta_time)
{
	static float	theta = 0;

	theta += (delta_time / 4);

	#ifndef ROT_Z
	ident_mat_4x4(mesh->rotation_mat_z);
	#else
	rot_matz_4x4(mesh->rotation_mat_z, theta);
	#endif
	
	#ifdef ROT_X
	rot_matx_4x4(mesh->rotation_mat_x, theta);
	#else
	ident_mat_4x4(mesh->rotation_mat_x);
	#endif
	
	#ifndef ROT_Y
	ident_mat_4x4(mesh->rotation_mat_y);
	#else
	rot_maty_4x4(mesh->rotation_mat_y, theta);
	#endif
	
}

void	mod_cube_rotation2(t_mesh *mesh, float delta_time)
{
	static float	theta = 0;

	theta += delta_time;

	#ifdef ROT_Z
	rot_matz_4x4(mesh->rotation_mat_z, theta);
	#else
	ident_mat_4x4(mesh->rotation_mat_z);
	#endif
	#ifdef ROT_X
	rot_matx_4x4(mesh->rotation_mat_x, theta);
	#else
	ident_mat_4x4(mesh->rotation_mat_x);
	#endif
	#ifndef ROT_Y
	ident_mat_4x4(mesh->rotation_mat_y);
	#else
	rot_maty_4x4(mesh->rotation_mat_y, theta);
	#endif
}
