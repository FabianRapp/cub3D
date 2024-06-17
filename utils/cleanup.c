#include "../includes/cub3D.h"

void	clean_mlx(t_main *main_data)
{
	if (main_data->mlx)
	{
		mlx_close_window(main_data->mlx);
		free_fps_digit_textures();
		if (main_data->img)
			mlx_delete_image(main_data->mlx, main_data->img);
		mlx_terminate(main_data->mlx);
		main_data->mlx = NULL;
		//printf("on linux MLX will always leak 12140 byte(s) leaked in 81 allocation(s) (with fsanitize)\n");
	}
}

void	free_mesh(t_mesh *mesh)
{
	int	i;

	i = 0;
	while (i < mesh->mtl_count)
	{
		i++;
	}
	free(mesh->mtl_libs);
	ft_free((void **)&mesh->triangles);
}

void	cleanup_exit(void *m_data)
{
	t_main	*main_data;
	int		i;

	main_data = (t_main *)m_data;
	fprintf(stderr, "exiting..\n");
	i = 0;
	while (i < main_data->mesh_count)
	{
		free_mesh(main_data->meshes + i);
		i++;
	}
	ft_free((void **)&main_data->meshes);
	free_menu(main_data, &main_data->menu);
	clean_mlx(main_data);
	exit(0);
}

