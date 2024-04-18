/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 08:36:34 by frapp             #+#    #+#             */
/*   Updated: 2024/04/18 12:52:35 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

typedef	struct s_obj_parser
{
	char		*path;
	int			fd;
	char		*line;
	int			vertex_count;
	int			normal_count;
	t_vec3		max;
	t_vec3		min;
	t_mesh		*mesh;
	t_vec3		*vertexes;
	t_vec3		*normals;
	t_triangle	*tris;
	int			tris_count;
	int			tris_size;
}	t_obj_parser;

void	obj_parser_count(t_obj_parser *vars)
{
	vars->fd = open(vars->path, O_RDONLY);
	vars->line = get_next_line(vars->fd, false);
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "v ", 2))
			vars->vertex_count++;
		else if (!ft_strncmp(vars->line, "vn ", 3))
			vars->normal_count++;
		free(vars->line);
		vars->line = get_next_line(vars->fd, false);
	}
	get_next_line(vars->fd, true);
	close(vars->fd);
}

float	str_to_float(char *str)
{
	int		integer;
	float	fraction;
	float	return_val;

	char *new = ft_strtrim(str, "\n");
	integer = ft_atoi(new);
	char *new2 = ft_strchr(new, '.');
	if (new2)
		new2++;
	if (!new2 || !ft_isdigit(*new2))
		return (free(new), (float)integer);
	fraction = (float)ft_atoi(new2);
	fraction /= ft_powint(10, ft_strlen(new2));
	return_val = fraction + integer;
	free(new);
	return (return_val);
}

int	obj_parser_fill_vertexes(t_obj_parser *vars)
{
	int		vertex_i;
	int		normal_i;
	char	**split;

	vars->fd = open(vars->path, O_RDONLY);
	vars->line = get_next_line(vars->fd, false);
	vertex_i = 0;
	normal_i = 0;
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "v ", 2))
		{
			split = ft_split(vars->line, ' ');
			vars->vertexes[vertex_i].x = str_to_float(split[1]);
			vars->vertexes[vertex_i].y = str_to_float(split[2]);
			vars->vertexes[vertex_i].z = str_to_float(split[3]);
			//print_vec3(vars->vertexes[vertex_i], NULL);
			ft_free_2darr(split);
			vertex_i++;
		}
		else if (!ft_strncmp(vars->line, "vn ", 3))
		{
			split = ft_split(vars->line, ' ');
			vars->normals[normal_i].x = str_to_float(split[1]);
			vars->normals[normal_i].y = str_to_float(split[2]);
			vars->normals[normal_i].z = str_to_float(split[3]);
			ft_free_2darr(split);
			printf("normal_i: %d; normal_count: %d\n", normal_i, vars->normal_count);
			normal_i++;
		}
		free(vars->line);
		vars->line = get_next_line(vars->fd, false);
	}
	get_next_line(vars->fd, true);
	close(vars->fd);
	return (vertex_i);
}

t_vec3	parse_face_vertex(t_obj_parser *vars, char *sub_face)
{
	t_vec3	return_vec;
	char	**split;

	split = ft_split(sub_face, '/');
	ft_memcpy(&return_vec, vars->vertexes + ft_atoi(split[0]), sizeof(t_vec3));
	ft_free_2darr(split);
	return(return_vec);
}

void	triangulation(t_obj_parser *vars, t_vec3 *vertexes, int vertex_count)
{
	t_triangle	*tris;
	int			tris_count;
	int			tris_i;
	t_vec3		*anchor = vertexes;

	if (!vertex_count)
		return ;
	tris_count = vertex_count - 2;
	tris = malloc(sizeof(t_triangle) * tris_count);
	tris_i = 0;
	while (tris_i < tris_count) {
		tris[tris_i].p[0] = vertexes[0];
		tris[tris_i].p[1] = vertexes[tris_i + 1];
		tris[tris_i].p[2] = vertexes[tris_i + 2];
		tris[tris_i].col = RED;
		tris_i++;
	}
	t_triangle	*tmp123;
	if (vars->tris_size < sizeof(t_triangle) * (vars->tris_count + 1) + sizeof(t_triangle) * tris_count)
	{
		tmp123 = ft_memjoin(vars->tris, tris, sizeof(t_triangle) * (vars->tris_count + 1), sizeof(t_triangle) * tris_count);
		free(vars->tris);
		vars->tris = tmp123;
		vars->tris_size = (sizeof(t_triangle) * (vars->tris_count + 1) + sizeof(t_triangle) * tris_count) * 2;
	}
	else
	{
		ft_memcpy(vars->tris + vars->tris_count, tris, sizeof(t_triangle) * tris_count);
	}
	vars->tris_count += tris_count;
}

void	obj_parser_handle_faces(t_obj_parser *vars)
{
	char		**split;
	int			count;
	t_vec3		tmp_v[100];
	t_triangle	tmp_tri[10];
	int			i;

	vars->tris = malloc(sizeof(t_triangle));
	vars->tris_size = sizeof(t_triangle);
	vars->fd = open(vars->path, O_RDONLY);
	vars->line = get_next_line(vars->fd, false);
	int nb = 0;
	while (vars->line)
	{
		printf("faces loop %d\n", nb++);
		if (!ft_strncmp(vars->line, "f ", 2))
		{
			split = ft_split(vars->line, ' ');
			count = 0;
			while (split[count])
			{
				count++;
			}
			ft_bzero(tmp_v, sizeof(tmp_v));
			i = 1;
			while (i < count)
			{
				tmp_v[i] = parse_face_vertex(vars, split[i]);
				i++;
			}
			ft_free_2darr(split);
			count--;
			if (count == 3)
			{
				ft_bzero(tmp_tri, sizeof(tmp_tri));
				ft_memcpy(tmp_tri[0].p + 0, tmp_v + 0, sizeof(t_vec3));
				ft_memcpy(tmp_tri[0].p + 1, tmp_v + 1, sizeof(t_vec3));
				ft_memcpy(tmp_tri[0].p + 2, tmp_v + 2, sizeof(t_vec3));
				t_triangle	*tmp123;
				tmp123 = ft_memjoin(vars->tris, tmp_tri, sizeof(t_triangle) * (vars->tris_count + 1), sizeof(t_triangle));
				free(vars->tris);
				vars->tris = tmp123;
				vars->tris[vars->tris_count].col = RED;
				vars->tris_count++;
			}
			else
			{
				triangulation(vars, tmp_v, count);
			}
		}
		free(vars->line);
		vars->line = get_next_line(vars->fd, false);
	}
	printf("faces loop end\n");
	get_next_line(vars->fd, true);
	close(vars->fd);
}

void	load_obj_file(char *path, t_mesh *mesh, t_main *main_data)
{
	t_obj_parser	vars;

	vars.path = path;
	vars.mesh = mesh;
	vars.tris = NULL;
	vars.tris_count = 0;
	vars.vertexes = NULL;
	vars.vertex_count = 0;
	vars.normals = NULL;
	vars.normal_count = 0;
	obj_parser_count(&vars);
	printf("vertex count: %d\n", vars.vertex_count);
	printf("normal count: %d\n", vars.normal_count);
	vars.vertexes = ft_calloc(vars.vertex_count, sizeof(t_vec3));
	vars.normals = malloc(vars.normal_count * sizeof(t_vec3));
	if (!vars.vertexes || (!vars.normals && vars.normal_count))
	{
		printf("mall err\n");
		exit(1);
	}
	vars.vertex_count = obj_parser_fill_vertexes(&vars);
	printf("parsed vertexes\n");
	obj_parser_handle_faces(&vars);
	printf("parsed faces\n");
	free(vars.vertexes);
	free(vars.normals);
	mesh->triangles = vars.tris;
	mesh->count = vars.tris_count;
	t_vec3	momentum = {0, 0, 10.0};
	mesh->momentum = momentum;
	float rotation_mat[4][4] = {
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	ft_memcpy(mesh->rotation_mat_x, rotation_mat, sizeof(rotation_mat));
	ft_memcpy(mesh->rotation_mat_y, rotation_mat, sizeof(rotation_mat));
	ft_memcpy(mesh->rotation_mat_z, rotation_mat, sizeof(rotation_mat));
	mesh->main = main_data;
	mesh->d_time = &main_data->mlx->delta_time;
	mesh->img = main_data->img;
	printf("tri count: %d\n", mesh->count);
}

