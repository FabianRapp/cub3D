/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   obj_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 08:36:34 by frapp             #+#    #+#             */
/*   Updated: 2024/04/24 11:15:56 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cub3D.h>
#include <MLX42.h>

FILE *fd;

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
	int			tris_size;
	t_mtl		**mtl_libs;
	int			mtl_libs_count;
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

float	str_to_float(char *str)
{
	int		integer;
	float	fraction;
	float	return_val;
	int		sign;

	sign = 1;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	char *new = ft_strtrim(str, "\n");
	integer = ft_atoi(new);
	char *new2 = ft_strchr(new, '.');
	if (new2 && *new2 == '.')
		new2++;
	else
		return (free(new), (float)integer);
	if (!new2 || !ft_isdigit(*new2))
		return (free(new), (float)integer);
	fraction = (float)ft_atoi(new2);
	fraction /= ft_powint(10, ft_strlen(new2));
	return_val = fraction + integer;
	free(new);
	return_val *= sign;
	return (return_val);
}

int	obj_parser_fill_vertexes(t_obj_parser *vars)
{
	int		vertex_i;
	int		normal_i;
	int		texture_cords_i;
	char	**split;

	vars->fd = open(vars->path, O_RDONLY);
	vars->line = get_next_line(vars->fd);
	vertex_i = 0;
	normal_i = 0;
	texture_cords_i = 0;
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "v ", 2))
		{
			split = ft_split(vars->line, ' ');
			//printf("vertex_i : %d\n", vertex_i);
			//printf("vertex coutn: %d\n", vars->vertex_count);
			vars->vertexes[vertex_i].x = str_to_float(split[1]);
			vars->vertexes[vertex_i].y = str_to_float(split[2]);
			vars->vertexes[vertex_i].z = str_to_float(split[3]);
			vars->vertexes[vertex_i].w = 1;
			fprintf(fd, "v %f %f %f\n", vars->vertexes[vertex_i].x, vars->vertexes[vertex_i].y, vars->vertexes[vertex_i].z);
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
			vars->normals[normal_i].w = 1;
			ft_free_2darr(split);
			//printf("normal_i: %d; normal_count: %d\n", normal_i, vars->normal_count);
			normal_i++;
		}
		else if (!ft_strncmp(vars->line, "vt ", 3))
		{
			split = ft_split(vars->line, ' ');
			vars->texture_cords[texture_cords_i].u = str_to_float(split[1]);
			vars->texture_cords[texture_cords_i].v = str_to_float(split[2]);
			vars->texture_cords[texture_cords_i].w = 1;
			if (split[3])
				vars->texture_cords[texture_cords_i].w = str_to_float(split[3]);
			ft_free_2darr(split);
			//printf("normal_i: %d; normal_count: %d\n", normal_i, vars->normal_count);
			texture_cords_i++;
		}
		free(vars->line);
		vars->line = get_next_line(vars->fd);
	}
	get_next_line(vars->fd);
	close(vars->fd);
	fprintf(fd, "\n");
	return (vertex_i);
}

t_vec3	parse_face_vertex(t_obj_parser *vars, char *sub_face, t_mtl *mtl)
{
	t_vec3	return_vec;
	char	**split;
	int		vt_index;

	split = ft_split(sub_face, '/');
	vt_index = ft_atoi(split[0]) - 1;
	fprintf(fd, "%d", vt_index + 1);
	ft_memcpy(&return_vec, vars->vertexes + vt_index, sizeof(t_vec3));
	//printf("vt_index: %d\n", vt_index);
	return_vec.w = 1;
	return_vec.mtl = mtl;
	if (split[1] && split[2])
	{
		vt_index = ft_atoi(split[1]) - 1;
		
		// printf("u: %f v: %f\n", vars->texture_cords[vt_index].u,  vars->texture_cords[vt_index].v);
		return_vec.u = vars->texture_cords[vt_index].u;
		return_vec.v = vars->texture_cords[vt_index].v;
		return_vec.w = vars->texture_cords[vt_index].w;
	}
	ft_free_2darr(split);
	return(return_vec);
}

t_vec3	parse_face_normals(t_obj_parser *vars, char *sub_face)
{
	t_vec3	return_vec = {0};
	char	**split;
	int 	i;

	if (!split || !split[0] || !split[1] || !split[2])
		return(return_vec);
	split = ft_split(sub_face, '/');
	i =  ft_atoi(split[1]) - 1;
	printf("index: %d\n", i);
	ft_memcpy(&return_vec, vars->normals + i, sizeof(t_vec3));
	ft_free_2darr(split);
	return(return_vec);
}

void	triangulation(t_obj_parser *vars, t_vec3 *vertexes, int vertex_count, t_vec3 *normals)
{
	t_triangle	*tris;
	int			tris_count;
	int			tris_i;
	int			vertex_i;
	t_vec3		*anchor = vertexes;
	static int	buffer_size = 0;
	int			min_buffer_size;

	static float last_z_len = 0;
	static float max_z_len = 0;

	if (!vertex_count)
		return ;
	tris_count = vertex_count - 2;
	min_buffer_size = (tris_count + vars->tris_count + 1) * sizeof(t_triangle);
	if (buffer_size < min_buffer_size)
	{
		buffer_size = 2 * min_buffer_size + 1;
		tris = ft_calloc(buffer_size, 1);
		ft_memcpy(tris, vars->tris, sizeof(t_triangle) * vars->tris_count);
		free(vars->tris);
		vars->tris = tris;
	}
	else
		tris = vars->tris;
	//printf("vars->tris_count: %d\n", vars->tris_count);
	tris_i = vars->tris_count;
	vars->tris_count += tris_count;
	vertex_i = 0;
	//printf("buffer size: %lu\n", buffer_size / sizeof(t_triangle));
	while (tris_i < vars->tris_count) {
		//printf("tris_i: %d\n", tris_i);
		tris[tris_i].p[0] = vertexes[0];
		tris[tris_i].p[1] = vertexes[vertex_i + 1];
		tris[tris_i].p[2] = vertexes[vertex_i + 2];
		//print_vec3(vertexes[vertex_i + 1], "first: ");
		//print_vec3(vertexes[vertex_i + 2], "second: ");
		tris[tris_i].obj_normal[0] = normals[0];
		tris[tris_i].obj_normal[1] = normals[vertex_i + 1];
		tris[tris_i].obj_normal[2] = normals[vertex_i + 2];

		tris[tris_i].normal = normals[0];
		// t_vec3	norms_sum = v3_add(v3_add(normals[0], normals[tris_i + 1]), normals[tris_i + 2]);
		// tris[tris_i].normal = v3_scale(norms_sum, 1 / length_vec3(&norms_sum));

		tris[tris_i].col = vars->colors[(vars->tris_count + tris_i) % OBJ_PARSER_COLOR_COUNT];
		vertex_i++;
		tris_i++;
	}
	//if (last_z_len != max_z_len)
		//printf("max z len: %f\n", max_z_len);
	last_z_len = max_z_len;
	// t_triangle	*tmp123;
	// if (vars->tris_size < sizeof(t_triangle) * (vars->tris_count + 1) + sizeof(t_triangle) * tris_count)
	// {
	// 	tmp123 = ft_memjoin(vars->tris, tris, sizeof(t_triangle) * (vars->tris_count + 1), sizeof(t_triangle) * tris_count);
	// 	free(vars->tris);
	// 	vars->tris = tmp123;
	// 	vars->tris_size = (sizeof(t_triangle) * (vars->tris_count + 1) + sizeof(t_triangle) * tris_count) * 2;
	// }
	// else
	// {
	// 	ft_memcpy(vars->tris + vars->tris_count, tris, sizeof(t_triangle) * tris_count);
	// }
}

void	parse_vt(t_obj_parser *vars, char *face_p, t_vec3 *vec)
{
	char	**split;
	int		vt_index;

	split = ft_split(face_p, '/');
	if (!split || !split[0] || !split[1] || !split[2])
		return ;
	vt_index = ft_atoi(split[1]) - 1;
	// printf("u: %f v: %f\n", vars->texture_cords[vt_index].u,  vars->texture_cords[vt_index].v);
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

	cur_mtl = NULL;
	vars->tris = malloc(sizeof(t_triangle));
	vars->tris_size = sizeof(t_triangle);
	vars->fd = open(vars->path, O_RDONLY);
	vars->line = get_next_line(vars->fd);
	int nb = 0;
	while (vars->line)
	{
		if (!ft_strncmp(vars->line, "usemtl ", ft_strlen("usemtl ")))
		{
			tmp = ft_strtrim(vars->line + ft_strlen("usemtl "), "\n");
			cur_mtl = get_mtl(vars, tmp);
			//printf("texture probe: %u\n", cur_mtl->texture->pixels[1000]);
			free(tmp);
		}
		if (!ft_strncmp(vars->line, "f ", 2))
		{
			split = ft_split(vars->line, ' ');
			count = 0;
			while (split[count])
			{
				count++;
			}
			ft_bzero(tmp_v, sizeof(tmp_v));
			count--;
			i = 0;
			fprintf(fd, "f");
			while (i < count)
			{
				fprintf(fd, " ");
				tmp_v[i] = parse_face_vertex(vars, split[i + 1], cur_mtl);
				tmp_v_norm[i] = parse_face_normals(vars, split[i + 1]);
				//parse_vt(vars, split[i], tmp_v + i - 1);
				//print_vec3(tmp_v[i - 1], NULL);
				i++;
			}
			fprintf(fd, "\n");
			ft_free_2darr(split);
			
			triangulation(vars, tmp_v, count, tmp_v_norm);
			// else
			// {
			// 	printf("parser error\n");
			// 	exit(1);
			// }
		}
		//printf("hre\n");
		free(vars->line);
		vars->line = get_next_line(vars->fd);
	}
	//printf("faces loop end\n");
	get_next_line(vars->fd);
	close(vars->fd);
}



// l 194 409
// l 194 1146
void	sacle_vecs(t_obj_parser *vars)
{
	int		i;
	t_vec3	scalar;
	t_vec3	translate;
	t_vec3	rotation;
	if (!ft_strcmp(vars->path, "RAN Easter Egg 2024 - OBJ/RAN_Easter_Egg_2024_High_Poly.obj")
		|| !ft_strcmp(vars->path, "RAN Easter Egg 2024 - OBJ/RAN_Easter_Egg_2024_Low_Poly.obj"))
	{
		init_vec3(&scalar, 100.0f, 100.0f, 100.0f, 1);
		init_vec3(&translate, 0.0f, 5.0f, -5.0f, 0.0f);
		init_vec3(&rotation, 0.0f, M_PI_2 , 0.0f, 0);
	}
	else if (!ft_strcmp(vars->path, "objs/HorseArmor.obj"))
	{
		init_vec3(&scalar, 5.0f, 5.0f, 5.0f, 5.0f);
		init_vec3(&translate, 0.0f, 0.0f, 1.0f, 0.0f);
		init_vec3(&rotation, 1.5f, 3.2f, 0.0f, 0.0f);
	}
	else
	{
		//init_vec3(&scalar, 0.1f, 0.1f, 0.1f, 1.0f);
		init_vec3(&scalar, 0.25, 0.25, 0.25, 1);
		init_vec3(&translate, 0.0f, 0.0f, 50.0f, 0.0f);
		init_vec3(&rotation, 0, M_PI_2, 0, 0.0f);
	}
	i = 0;
	t_vec3	max = {0};
	t_vec3 min = {.x = 10000000.0f, .y=100000000.0f, .z=100000000.0f};
	while (i < vars->vertex_count)
	{
		// if (vars->vertexes[i].z < 0.01f)// && vars->vertexes[i].x > -0.01f)
		// 	vars->vertexes[i].z = 0.01f;
		if (vars->vertexes[i].x > max.x)
			max.x = vars->vertexes[i].x;
		if (vars->vertexes[i].y > max.y)
			max.y = vars->vertexes[i].y;
		if (vars->vertexes[i].z > max.z)
			max.z = vars->vertexes[i].z;
		if (vars->vertexes[i].x < min.x)
			min.x = vars->vertexes[i].x;
		if (vars->vertexes[i].y < min.y)
			min.y = vars->vertexes[i].y;
		if (vars->vertexes[i].z < min.z)
		{
			//printf("min z i: %d\n", i);
			min.z = vars->vertexes[i].z;
		}
		//if (i == 194 || i == 409 || i == 1146)
	//	{
			//printf("vertex index %d: ", i);
			//print_vec3(vars->vertexes[i], NULL);
		//}
		rotate_vec3(vars->vertexes + i, rotation.x, rotation.y, rotation.z);
		multiply_vec3(vars->vertexes + i, &scalar);
		add_vec3(vars->vertexes + i, &translate);
		i++;
	}
	printf("total min max off all vertexes:\nmax: x: %f y: %f z: %f\nmin: x: %f y: %f z: %f\n", max.x, max.y, max.z, min.x, min.y, min.z);
	i = 0;
	while (i < vars->normal_count)
	{
		rotate_vec3(vars->normals + i, rotation.x, rotation.y, rotation.z);
		i++;
	}
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

void	fill_3_floats(char *str, float arr[3])
{
	int	i;

	i = 0;
	while (i < 3)
	{
		arr[i] = str_to_float(str);
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
				arr[i].ns = str_to_float(line + ft_strlen("Ns "));
			else if (!ft_strncmp(line, "Ni ", ft_strlen("Ni ")))
				arr[i].ni = str_to_float(line + ft_strlen("Ni "));
			else if (!ft_strncmp(line, "d ", ft_strlen("d ")))
				arr[i].d = str_to_float(line + ft_strlen("d "));
			else if (!ft_strncmp(line, "illum ", ft_strlen("illum ")))
				arr[i].illum = str_to_float(line + ft_strlen("illum "));
			else if (!ft_strncmp(line, "Ka ", ft_strlen("Ka ")))
				fill_3_floats(line + ft_strlen("Ka "), arr[i].ka);
			else if (!ft_strncmp(line, "Ks ", ft_strlen("Ks ")))
				fill_3_floats(line + ft_strlen("Ks "), arr[i].ks);
			else if (!ft_strncmp(line, "Ke ", ft_strlen("Ke ")))
				fill_3_floats(line + ft_strlen("Ke "), arr[i].ke);
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
				printf("texture: %s\n", tmp);
			else
			{
				//printf("texture probe: %u\n", arr[i].texture->pixels[1000]);
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
		// for (int j = 0; j <= i; j++)
		// {
		// 	for (int k = 0; !is_buffer_all_zeros(vars->mtl_libs[j] + k, sizeof(t_mtl)); k++)
		// 	{
		// 		printf("%u\n", vars->mtl_libs[j][k].texture->pixels[0]);
		// 	}
		// }
		i++;
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	free(line);
}

void	load_obj_file(char *dir, char *path, t_mesh *mesh, t_main *main_data)
{
	t_obj_parser	vars;

	fd = fopen("log.log", "a");

	vars.path = path;
	vars.mesh = mesh;
	vars.tris = NULL;
	vars.tris_count = 0;
	vars.vertexes = NULL;
	vars.vertex_count = 0;
	vars.normals = NULL;
	vars.normal_count = 0;
	vars.texture_cords_count = 0;
	vars.texture_cords = NULL;
	mesh->obj_file = true;
	vars.mtl_libs = NULL;
	vars.mtl_libs_count = 0;
	init_obj_file_colors(&vars);
	obj_parser_count(&vars);
	vars.vertexes = ft_calloc(vars.vertex_count, sizeof(t_vec3));
	vars.normals = ft_calloc(vars.normal_count, sizeof(t_vec3));
	vars.texture_cords = ft_calloc(vars.texture_cords_count, sizeof(t_vec3));
	vars.mtl_libs = ft_calloc(vars.mtl_libs_count + 1, sizeof(t_mtl));
	if ((!vars.vertexes && vars.vertex_count) || (!vars.normals && vars.normal_count))
	{
		printf("mall err(vertex count: %d normal count (is not 0): %d\n", vars.vertex_count, vars.normal_count);
		exit(1);
	}
	obj_parser_parse_mtl_libs(&vars, dir, path);
	printf("mtl parsed\n");
	//vars.vertex_count = 
	obj_parser_fill_vertexes(&vars);
	sacle_vecs(&vars);
	printf("parsed vertexes\n");
	obj_parser_handle_faces(&vars);
	printf("parsed faces\n");


	mesh->triangles = vars.tris;
	mesh->count = vars.tris_count;
	t_vec3	momentum = {0.5, 0.3, 0.0};
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
	// for (int i = 1; i < vars.vertex_count; i++)
	// {
	// 	printf("vec %d: ", i);
	// 	print_vec3(vars.vertexes[i], 0);
	// }
	// for (int i = 0; i < vars.tris_count; i++)
	// {
	// 	printf("========\ntri %d:\n", i);
	// 	print_vec3(vars.tris[i].obj_normal[0], "1 :");
	// 	print_vec3(vars.tris[i].obj_normal[1], "2 :");
	// 	print_vec3(vars.tris[i].obj_normal[2], "3 :");
	// }
//	printf("vert count: %d\n", vars.vertex_count);
	printf("tri count: %d\n", mesh->count);
	fclose(fd);
	free(vars.vertexes);
	free(vars.normals);
}

