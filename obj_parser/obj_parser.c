/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 08:36:34 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"
#include "../MLX42/include/MLX42/MLX42.h"

typedef	struct s_obj_parser
{
	char		*path;
	int			fd;
	char		*line;
	int			vertex_count;
	int			normal_count;
	int			texture_cords_count;
	t_vec3		*texture_cords;
	t_vec3		max;
	t_vec3		min;
	t_mesh		*mesh;
	t_vec3		*vertexes;
	t_vec3		*normals;
	t_triangle	*tris;
	int			tris_count;
	t_mtl		**mtl_libs;
	int			mtl_libs_count;
	int			buffer_size;
	uint32_t	colors[OBJ_PARSER_COLOR_COUNT];
}	t_obj_parser;

t_mtl	*get_mtl(t_obj_parser *vars, char *name)
{
	int	lib_i;
	int	i;

	lib_i = 0;
	while (lib_i < vars->mtl_libs_count)
	{
		i = 0;
		while (!is_buffer_all_zeros(vars->mtl_libs[lib_i] + i, sizeof(t_mtl)))
		{
			if (!ft_strcmp(vars->mtl_libs[lib_i][i].name, name))
				return (vars->mtl_libs[lib_i] + i);
			i++;
		}
		lib_i++;
	}
	return (NULL);
}

void	obj_parser_count(t_obj_parser *vars)
{
	vars->fd = open(vars->path, O_RDONLY);
	if (vars-> fd < 0 || errno)
	{
		printf("%s\n", getenv("PWD"));
		printf("err: %s %s %d %s\n", strerror(errno), __FILE__, __LINE__, vars->path);
		exit(errno);
	}

	vars->line = get_next_line(vars->fd);
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "v ", 2))
			vars->vertex_count++;
		else if (!ft_strncmp(vars->line, "vn ", 3))
			vars->normal_count++;
		else if (!ft_strncmp(vars->line, "mtllib ", ft_strlen("mtllib ")))
			vars->mtl_libs_count++;
		else if (!ft_strncmp(vars->line, "vt ", ft_strlen("vt ")))
			vars->texture_cords_count++;
		free(vars->line);
		vars->line = get_next_line(vars->fd);
	}
	get_next_line(vars->fd);
	close(vars->fd);
}


double	str_to_double(char *str)
{
	int		integer;
	double	fraction;
	int		sign;

	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	integer = ft_atoi(str);
	while (ft_isdigit(*str))
		str++;
	if (*str == '.')
		str++;
	else
		return ((double)integer * sign);
	if (!ft_isdigit(*str))
		return ((double)integer * sign);
	fraction = (double)ft_atoi(str);
	if (ft_strchr(str, '\n'))
		fraction /= ft_powint(10, ft_strlen(str) - 1);
	else
		fraction /= ft_powint(10, ft_strlen(str));
	return ((fraction + integer) * sign);
}


int	obj_parser_fill_vertexes(t_obj_parser *vars)
{
	int		vertex_i;
	int		normal_i;
	int		texture_cords_i;
	char	**split;

	vars->fd = open(vars->path, O_RDONLY);
	if (vars-> fd < 0 || errno)
	{
		printf("err: %s %s %d %s\n", strerror(errno), __FILE__, __LINE__, vars->path);
		exit(errno);
	}

	vars->line = get_next_line(vars->fd);
	vertex_i = 0;
	normal_i = 0;
	texture_cords_i = 0;
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "v ", 2))
		{
			split = ft_split(vars->line, ' ');
			vars->vertexes[vertex_i].x = str_to_double(split[1]);
			vars->vertexes[vertex_i].y = str_to_double(split[2]);
			vars->vertexes[vertex_i].z = str_to_double(split[3]);
			vars->vertexes[vertex_i].w = 1;
			ft_free_2darr(split);
			vertex_i++;
		}
		else if (!ft_strncmp(vars->line, "vn ", 3))
		{
			split = ft_split(vars->line, ' ');
			vars->normals[normal_i].x = str_to_double(split[1]);
			vars->normals[normal_i].y = str_to_double(split[2]);
			vars->normals[normal_i].z = str_to_double(split[3]);
			vars->normals[normal_i].w = 1;
			ft_free_2darr(split);
			normal_i++;
		}
		else if (!ft_strncmp(vars->line, "vt ", 3))
		{
			split = ft_split(vars->line, ' ');
			vars->texture_cords[texture_cords_i].u = str_to_double(split[1]);
			vars->texture_cords[texture_cords_i].v = str_to_double(split[2]);
			vars->texture_cords[texture_cords_i].w = 1;
			if (split[3])
				vars->texture_cords[texture_cords_i].w = str_to_double(split[3]);
			ft_free_2darr(split);
			texture_cords_i++;
		}
		free(vars->line);
		vars->line = get_next_line(vars->fd);
	}
	get_next_line(vars->fd);
	close(vars->fd);
	return (vertex_i);
}

t_vec3	parse_face_vertex(t_obj_parser *vars, char *sub_face, t_mtl *mtl)
{
	t_vec3	return_vec;
	char	**split;
	int		v_index;
	int		vtexture_index;

	split = ft_split(sub_face, '/');
	v_index = ft_atoi(split[0]) - 1;
	ft_memcpy(&return_vec, vars->vertexes + v_index, sizeof(t_vec3));
	return_vec.w = 1;
	return_vec.mtl = mtl;
	if (split[1] && split[2])
	{
		vtexture_index = ft_atoi(split[1]) - 1;
		return_vec.u = vars->texture_cords[vtexture_index].u;
		return_vec.v = vars->texture_cords[vtexture_index].v;
		return_vec.w = vars->texture_cords[vtexture_index].w;
	}
	ft_free_2darr(split);
	return(return_vec);
}

t_vec3	parse_face_normals(t_obj_parser *vars, char *sub_face)
{
	t_vec3	return_vec = {0};
	char	**split;
	int 	i;

	split = ft_split(sub_face, '/');
	if (!split || !split[0] || !split[1] || !split[2])
	{
		ft_free_2darr(split);
		return(return_vec);
	}
	i =  ft_atoi(split[2]) - 1;
	//printf("i: %d\nvars->normal_count: %d\n", i, vars->normal_count);
	return_vec = vars->normals[i];
	ft_free_2darr(split);
	return(return_vec);
}

void	triangulation(t_obj_parser *vars, t_vec3 *vertexes, int vertex_count, t_vec3 *normals)
{
	t_triangle	*tris;
	int			local_tris_count;
	int			tris_i;
	int			vertex_i;
	int			min_buffer_size;

	if (!vertex_count)
		return ;
	local_tris_count = vertex_count - 2;
	min_buffer_size = (local_tris_count + vars->tris_count + 1) * sizeof(t_triangle);
	if (vars->buffer_size < min_buffer_size)
	{
		vars->buffer_size = 2 * min_buffer_size + 1;
		tris = ft_calloc(vars->buffer_size, 1);
		ft_memcpy(tris, vars->tris, sizeof(t_triangle) * vars->tris_count);
		free(vars->tris);
		vars->tris = tris;
	}
	else
		tris = vars->tris;
	tris_i = vars->tris_count;
	vars->tris_count += local_tris_count;
	vertex_i = vertex_count - 1;
	while (tris_i < vars->tris_count)
	{
		ft_memcpy(tris[tris_i].p, vertexes, sizeof(t_vec3) * 3);
		tris[tris_i].p[0] = vertexes[0];
		tris[tris_i].p[1] = vertexes[vertex_i];
		tris[tris_i].p[2] = vertexes[vertex_i - 1];
		tris[tris_i].normals[0] = normals[0];
		tris[tris_i].normals[1] = normals[vertex_i];
		tris[tris_i].normals[2] = normals[vertex_i - 1];
		tris[tris_i].col = vars->colors[(vars->tris_count + tris_i) % OBJ_PARSER_COLOR_COUNT];
		vertex_i--;
		tris_i++;
	}
}

void	parse_vt(t_obj_parser *vars, char *face_p, t_vec3 *vec)
{
	char	**split;
	int		vt_index;

	split = ft_split(face_p, '/');
	if (!split || !split[0] || !split[1] || !split[2])
	{
		ft_free_2darr(split);
		return ;
	}
	vt_index = ft_atoi(split[1]) - 1;
	vec->u = vars->texture_cords[vt_index].u;
	vec->v = vars->texture_cords[vt_index].v;
	vec->w = vars->texture_cords[vt_index].w;
	ft_free_2darr(split);
}

void	obj_parser_handle_faces(t_obj_parser *vars)
{
	char		**split;
	int			count;
	t_vec3		tmp_v[100];
	t_vec3		tmp_v_norm[100];
	t_triangle	tmp_tri[10];
	int			i;
	t_mtl		*cur_mtl;
	char		*tmp;
	int			face_count = 0;

	cur_mtl = NULL;
	vars->fd = open(vars->path, O_RDONLY);
	if (vars-> fd < 0 || errno)
	{
		printf("err: %s %s %d %s\n", strerror(errno), __FILE__, __LINE__, vars->path);
		exit(errno);
	}

	vars->line = get_next_line(vars->fd);
	int nb = 0;
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "usemtl ", ft_strlen("usemtl ")))
		{
			tmp = ft_strtrim(vars->line + ft_strlen("usemtl "), "\n");
			printf("cur_mtl: %s\n", tmp);
			cur_mtl = get_mtl(vars, tmp);
			free(tmp);
		}
		if (!ft_strncmp(vars->line, "f ", 2))
		{
			face_count++;
			split = ft_split(vars->line, ' ');
			assume(split[0][0] == 'f');
			count = 0;
			while (split[count])
			{
				count++;
			}
			ft_bzero(tmp_v, sizeof(tmp_v));
			count--;
			i = 0;
			ft_bzero(tmp_v_norm, sizeof(tmp_v_norm));
			while (i < count)
			{
				tmp_v[i] = parse_face_vertex(vars, split[i + 1], cur_mtl);
				tmp_v_norm[i] = parse_face_normals(vars, split[i + 1]);
				i++;
			}
			ft_free_2darr(split);
			triangulation(vars, tmp_v, count, tmp_v_norm);
		}
		free(vars->line);
		vars->line = get_next_line(vars->fd);
	}
	get_next_line(vars->fd);
	close(vars->fd);
}

void	init_obj_file_colors(t_obj_parser *vars)
{
	vars->colors[0] = RED;
	vars->colors[1] = GREEN;
	vars->colors[2] = BLUE;
	vars->colors[3] = MAGENTA;
	vars->colors[4] = YELLOW;
	vars->colors[5] = WHITE;
	vars->colors[6] = CYAN;
	vars->colors[7] = PURPLE;
	vars->colors[8] = ORANGE;
	vars->colors[9] = TEAL;
	vars->colors[10] = PINK;
	vars->colors[11] = LIME;
}

int	mtl_len(char *file_path)
{
	int		fd;
	char	*line;
	int		len;

	fd = open(file_path, O_RDONLY);
	if ( fd < 0 || errno)
	{
		printf("err: %s %s %d %s\n", strerror(errno), __FILE__, __LINE__, file_path);
		exit(errno);
	}

	line = get_next_line(fd);
	len = 0;
	while (line)
	{
		while (line && ft_strncmp(line, "newmtl ", ft_strlen("newmtl ")))
		{
			free(line);
			line = get_next_line(fd);
		}
		if (line)
			len++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	return (len);
}

void	fill_3_doubles(char *str, double arr[3])
{
	int	i;

	i = 0;
	while (i < 3)
	{
		arr[i] = str_to_double(str);
		while (ft_isdigit(*str))
		{
			str++;
		}
		if (*str == '.')
		{
			str++;
			while (ft_isdigit(*str))
			{
				str++;
			}
		}
		str++;
		i++;
	}
}

// file_name has to be alloacted without \n
t_mtl	*parse_mtl(char *dir, char *file_name)
{
	t_mtl	*arr;
	char	*file_path;
	int		fd;
	char	*line;
	int		i;
	int		count;
	char	*tmp;

	file_path = ft_strjoin(dir, file_name);
	count = mtl_len(file_path);
	arr = ft_calloc(count + 1, sizeof(t_mtl));
	fd = open(file_path, O_RDONLY);
	if ( fd < 0 || errno)
	{
		printf("err: %s %s %d %s\n", strerror(errno), __FILE__, __LINE__, file_path);
		exit(errno);
	}
	free(file_path);
	line = get_next_line(fd);

	i = 0;
	while (line)
	{
		while (line && ft_strncmp(line, "newmtl ", ft_strlen("newmtl ")))
		{
			free(line);
			line = get_next_line(fd);
		}
		arr[i].index = i;
		arr[i].lib_name = file_name;
		arr[i].name = ft_strtrim(line + ft_strlen("newmtl "), "\n");
		free(line);
		line = get_next_line(fd);
		while (line && ft_strncmp(line, "newmtl ", ft_strlen("newmtl ")))
		{
			if (!ft_strncmp(line, "Ns ", ft_strlen("Ns ")))
				arr[i].ns = str_to_double(line + ft_strlen("Ns "));
			else if (!ft_strncmp(line, "Ni ", ft_strlen("Ni ")))
				arr[i].ni = str_to_double(line + ft_strlen("Ni "));
			else if (!ft_strncmp(line, "d ", ft_strlen("d ")))
				arr[i].d = str_to_double(line + ft_strlen("d "));
			else if (!ft_strncmp(line, "illum ", ft_strlen("illum ")))
				arr[i].illum = str_to_double(line + ft_strlen("illum "));
			else if (!ft_strncmp(line, "Ka ", ft_strlen("Ka ")))
				fill_3_doubles(line + ft_strlen("Ka "), arr[i].ka);
			else if (!ft_strncmp(line, "Ks ", ft_strlen("Ks ")))
				fill_3_doubles(line + ft_strlen("Ks "), arr[i].ks);
			else if (!ft_strncmp(line, "Ke ", ft_strlen("Ke ")))
				fill_3_doubles(line + ft_strlen("Ke "), arr[i].ke);
			else if (!ft_strncmp(line, "map_Kd ", ft_strlen("map_Kd ")))
				arr[i].map_kd = ft_strtrim(line + ft_strlen("map_Kd "), "\n");
			free(line);
			line = get_next_line(fd);
		}
		if (arr[i].map_kd)
		{
			tmp = ft_strjoin(dir, arr[i].map_kd);
			arr[i].texture = mlx_load_png(tmp);
			if (!arr[i].texture)
				fprintf(stderr, "texture: %s\n", tmp);
			else
			{
				//fprintf(stderr, "texture probe: %u\n", arr[i].texture->pixels[1000]);
			}
			free(tmp);
		}
		i++;
	}
	return (arr);
}

void	obj_parser_parse_mtl_libs(t_obj_parser *vars, char *dir, char *path)
{
	int		fd;
	char	*line;
	int		i;

	fd = open(path, O_RDONLY);
	if ( fd < 0 || errno)
	{
		printf("err: %s %s %d %s\n", strerror(errno), __FILE__, __LINE__, path);
		exit(errno);
	}

	line = get_next_line(fd);
	i = 0;
	while (line && i < vars->mtl_libs_count)
	{
		while (line && ft_strncmp(line, "mtllib ", ft_strlen("mtllib ")))
		{
			free(line);
			line = get_next_line(fd);
		}
		if (!line)
			break ;
		vars->mtl_libs[i] = parse_mtl(dir, ft_strtrim(line + ft_strlen("mtllib "), "\n"));
		i++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	free(line);
}

void	free_mesh2(t_mesh *mesh)
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

t_mesh	load_obj_file(char *dir, char *path, t_main *main_data)
{
	t_obj_parser	vars;
	t_mesh			mesh;

	errno = 0;
	ft_bzero(&vars, sizeof(vars));
	init_basic_data_mesh(main_data, &mesh);
	vars.path = path;
	vars.mesh = &mesh;
	mesh.obj_file = true;
	init_obj_file_colors(&vars);
	obj_parser_count(&vars);

	vars.vertexes = ft_calloc(vars.vertex_count, sizeof(t_vec3));
	vars.normals = ft_calloc(vars.normal_count, sizeof(t_vec3));
	vars.texture_cords = ft_calloc(vars.texture_cords_count, sizeof(t_vec3));
	vars.mtl_libs = ft_calloc(vars.mtl_libs_count, sizeof(t_mtl *));
	mesh.mtl_libs = vars.mtl_libs;
	mesh.mtl_count = vars.mtl_libs_count;
	if ((!vars.vertexes && vars.vertex_count) || (!vars.normals && vars.normal_count))
	{
		fprintf(stderr, "mall err\n");
		free(vars.vertexes);
		free(vars.normals);
		free(vars.texture_cords);
		free(vars.mtl_libs);
		cleanup_exit(main_data);
	}
	obj_parser_parse_mtl_libs(&vars, dir, path);
	obj_parser_fill_vertexes(&vars);
	obj_parser_handle_faces(&vars);
	mesh.triangles = vars.tris;
	mesh.count = vars.tris_count;
	printf("parser count %s: %d\n", path, mesh.count);
	mesh.main = main_data;
	mesh.img = main_data->img;
	free(vars.vertexes);
	free(vars.normals);
	printf("%s loaded\n", path);
	assert(!errno);
	//free(vars.texture_cords);
	return (mesh);
}

