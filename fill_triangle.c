/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_triangle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 01:38:58 by frapp             #+#    #+#             */
/*   Updated: 2024/06/26 17:28:50 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

void	sort_vertexes_for_y(t_triangle *tri)
{
	t_vec3	tmp;
	t_vec3	*p;

	p = tri->p;
	if (p[0].y > p[1].y)
	{
		tmp = p[0];
		p[0] = p[1];
		p[1] = tmp;
	}
	if (p[1].y > p[2].y)
	{
		tmp = p[1];
		p[1] = p[2];
		p[2] = tmp;
		if (p[0].y > p[1].y)
		{
			tmp = p[0];
			p[0] = p[1];
			p[1] = tmp;
		}
	}
}

/*
void	abs_uv(t_vec3	*p)
{
	p[0].u = fabs(p[0].u);
	p[0].v = fabs(p[0].v);
	p[1].u = fabs(p[1].u);
	p[1].v = fabs(p[1].v);
	p[2].u = fabs(p[2].u);
	p[2].v = fabs(p[2].v);
}
*/


static inline uint32_t	load_pixel_from_mlx_texture(uint32_t x, uint32_t y, uint32_t *buffer, uint32_t width)
{
	assume(x >= 0 && y >= 0 && x < width);
	return (buffer[width * y + x]);
}

static inline t_vec3	v3_add_incl_uv_inline(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	//v.x = a.x + b.x;
	//v.y = a.y + b.y;
	v.z = a.z + b.z;
	v.u = a.u + b.u;
	v.v = a.v + b.v;
	return (v);
}

static inline t_vec3	v3_sub_incl_uv_inline(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v.x = a.x - b.x;
	//if (v.x >= 0.0)
	//	v.x += 1.0;
	//else
	//	v.x -= 1.0;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	v.u = a.u - b.u;
	v.v = a.v - b.v;
	return (v);
}

static inline t_vec3 v3_scale_incl_uv_inline(t_vec3 a, double scalar)
{
	t_vec3	v;

	v.x = a.x * scalar;
	v.y = a.y * scalar;
	v.z = a.z * scalar;
	v.u = a.u * scalar;
	v.v = a.v * scalar;
	return v;
}

typedef struct s_vec3_fixed
{
	t_fixed	x;
	t_fixed	y;
	t_fixed	z;
	t_fixed	u;
	t_fixed	v;
}	t_vec3_fixed;

typedef struct s_most_inner_loop_vars
{
	uint16_t				abs_len_x;
	uint32_t				row_start_offset;
	double					len_x;
	int						first_col;
	float					*const depth;
	uint64_t				*const depth_bit_array;
	t_vec3					first_pixel_in_row;
	t_vec3					last_pixel_in_row;
	double					z_diff;
	uint32_t				*const pixels;
	int8_t					direct_x				: 2;
	const t_trimmed_texture	texture;
}	t_most_inner_loop_vars;

// todo: change loops later so this does not need to be used in this way
static inline t_arr_val	get_blocked_val_inline(t_blocked_arr arr, uint32_t x, uint32_t y)
{
	uint32_t	index_in_block = x % BLOCK_WIDTH + (y % BLOCK_HEIGHT) * BLOCK_WIDTH;
	uint32_t	block_col = x / BLOCK_WIDTH;
	uint32_t	block_row = y / BLOCK_HEIGHT;
	uint32_t	block_nb = block_col + block_row * arr.blocks_per_row;
	return (arr.arr[index_in_block + block_nb * BLOCK_SIZE]);
}

//depth_z_buffer is to save significant time in buffer reseting
//todo:
//	-the blocke array should be indexed better (not with u v)
//	-try if fixed point ops are better
//	-make it so direct_x is always 1 before calling this to allow optimizatons
static inline void	inner_loop(const t_most_inner_loop_vars vars)
{
	uint16_t	cur_x;
	float		cur_z;
	float		progress_per_step;
	float		diff_u = vars.last_pixel_in_row.u - vars.first_pixel_in_row.u;
	float		diff_v = vars.last_pixel_in_row.v - vars.first_pixel_in_row.v;
	float		step_u;
	float		step_v;
	float		step_z;

	cur_x = vars.first_col;
	uint32_t	fin_index = vars.first_col + vars.row_start_offset;
	progress_per_step = 1.0 / vars.abs_len_x;
	step_u = progress_per_step * diff_u;
	step_v = progress_per_step * diff_v;
	step_z = progress_per_step * vars.z_diff;
	cur_z = vars.first_pixel_in_row.z;
	float	cur_u = vars.first_pixel_in_row.u;
	float	cur_v = vars.first_pixel_in_row.v;
	for (int i = 0; i < vars.abs_len_x; i++)
	{
		uint32_t	bit_array_index = fin_index >> 3;//todo make fin_index a union to cur this
		uint8_t		bit_mask = 1 << (fin_index % 8);
#ifndef NDEBUG
		if (fin_index >= WIDTH * HEIGHT)
		{
			printf("fin_index: %u (%u > WIDTH * HEIGHT)\n", fin_index, fin_index - WIDTH * HEIGHT);
		}
#endif
		if (cur_z < vars.depth[fin_index]
			|| !(vars.depth_bit_array[bit_array_index] & bit_mask))
		{
			//if (!(vars.depth_bit_array[bit_array_index] & bit_mask))
				vars.depth_bit_array[bit_array_index] |= bit_mask;
			vars.depth[fin_index] = cur_z;
			vars.pixels[fin_index] = get_blocked_val_inline(vars.texture.buffer, ((int)(cur_u * vars.texture.max_width_index)),
											((int)(cur_v * vars.texture.max_height_index)));
		}
		fin_index += vars.direct_x;
		cur_z += step_z;
		cur_u += step_u;
		cur_v += step_v;
		//__builtin_prefetch(vars.depth + fin_index + vars.texture.width, 1, 1);
		//__builtin_prefetch(vars.depth + fin_index - vars.texture.width, 1, 1);
	}
}

void	fill_triangle_texture(mlx_image_t *img, t_triangle *projected, t_mesh *mesh, t_light_argb_stren color_sclars)
{
	t_vec3	*p = projected->p;
	t_most_inner_loop_vars	inner_vars __attribute__((__aligned__(16))) = {
										.texture = projected->p->mtl->texture,
										.pixels = (uint32_t *)(img->pixels),
										.depth = mesh->main->depth,
										.depth_bit_array = mesh->main->depth_bit_array,
									};
	sort_vertexes_for_y(projected);
	assume(p[0].y <= p[1].y && p[1].y <= p[2].y);
	for (int i = 0; i < 3; i++)
	{
		assume((int)round(p[i].x) >= 0);
		assume((int)round(p[i].x) < WIDTH);
		assume((int)round(p[i].y) >= 0.0);
		assume((int)round(p[i].y) < HEIGHT);
	}
	t_vec3	diff_20 = v3_sub_incl_uv_inline(p[2], p[0]);
	if (zero_f(diff_20.y))
		return ;
	t_vec3	diff_10 = v3_sub_incl_uv_inline(p[1], p[0]);

	double	cur_y_lf = p[0].y;
	double	total_y_progress;
	int row_index =  (int)round(cur_y_lf);
	double section_y_progress = 0;
	double	section_y_progress_per_step;
	double	total_y_progress_per_step;
	total_y_progress_per_step = 1.0 / diff_20.y;
	total_y_progress = 0.0;
	//int q = 0;
	if (!zero_f(diff_10.y))
	{
		section_y_progress = 0.0;
		section_y_progress_per_step = 1.0 / diff_10.y;
		while (section_y_progress < 1.0)
		{
			inner_vars.first_col = (int)round(diff_20.x * total_y_progress + p[0].x);
			int	last_col = (int)round(diff_10.x * section_y_progress + p[0].x);
			inner_vars.first_pixel_in_row = v3_scale_incl_uv_inline(diff_20, total_y_progress);
			inner_vars.first_pixel_in_row = v3_add_incl_uv_inline(inner_vars.first_pixel_in_row, p[0]);
			inner_vars.last_pixel_in_row = v3_scale_incl_uv_inline(diff_10, section_y_progress);
			inner_vars.last_pixel_in_row = v3_add_incl_uv_inline(inner_vars.last_pixel_in_row, p[0]);
			inner_vars.row_start_offset = WIDTH * row_index;
			if (inner_vars.first_col <= last_col)
			{
				//assume(!q || (inner_vars.direct_x == 1));
				inner_vars.direct_x = 1;
			}
			else
			{
				inner_vars.direct_x = -1;
			}
			//q++;
			//if ((inner_vars.direct_x == 1 && p[1].x < p[2].x)
			//	|| (inner_vars.direct_x == -1 && p[1].x > p[2].x))
			//{
			//	printf("direct_x: %d\n", inner_vars.direct_x);
			//	for(int i = 0; i < 3; i++)
			//	{
			//		char	str[2] = {0};
			//		str[0] = '0' + i;
			//		print_vec3(p[i], str);
			//	}
			//}
			//else if (p[1].x != p[2].x)
			//{
			//	printf("x\n");
			//}
			//if (p[1].x >= p[2].x)
			//	inner_vars.direct_x = -1;
			//else
			//	inner_vars.direct_x = 1;
			int	len_x = last_col - inner_vars.first_col + inner_vars.direct_x;
			inner_vars.z_diff = inner_vars.last_pixel_in_row.z - inner_vars.first_pixel_in_row.z;
			assume(len_x);
			inner_vars.abs_len_x = abs(len_x);
			inner_vars.len_x = (double)len_x;
			inner_loop(inner_vars);
			cur_y_lf = cur_y_lf + 1.0f;
			row_index =  (int)round(cur_y_lf);
			total_y_progress += total_y_progress_per_step;
			section_y_progress += section_y_progress_per_step;
		}
	}
	t_vec3	diff_21 = v3_sub_incl_uv_inline(p[2], p[1]);
	if (zero_f(round(diff_21.y)) || total_y_progress >= 1.0)
		return ;
	cur_y_lf = p[1].y;
	row_index = (int)round(cur_y_lf);
	section_y_progress_per_step = 1.0 / diff_21.y;
	section_y_progress = 0.0;
	while (section_y_progress < 1.0 && total_y_progress < 1.0)
	{
		assume(section_y_progress >= 0.0);
		assume(total_y_progress >= 0.0 && (section_y_progress <= total_y_progress || zero_f(section_y_progress - total_y_progress)));
		assume(row_index >= 0 && row_index < HEIGHT);
		inner_vars.first_col = (int)round(diff_20.x * total_y_progress + p[0].x);
		int last_col = (int)round(p[1].x + section_y_progress * diff_21.x);
		if (last_col > inner_vars.first_col)
			inner_vars.direct_x = 1;
		else
			inner_vars.direct_x = -1;
		inner_vars.first_pixel_in_row = v3_scale_incl_uv_inline(diff_20, total_y_progress);
		inner_vars.first_pixel_in_row = v3_add_incl_uv_inline(inner_vars.first_pixel_in_row, p[0]);
		inner_vars.last_pixel_in_row = v3_scale_incl_uv_inline(diff_21, section_y_progress);
		inner_vars.last_pixel_in_row = v3_add_incl_uv_inline(inner_vars.last_pixel_in_row, p[1]);

		inner_vars.z_diff = inner_vars.last_pixel_in_row.z - inner_vars.first_pixel_in_row.z;
		inner_vars. row_start_offset = WIDTH * row_index;
		assume(inner_vars.row_start_offset + WIDTH <= WIDTH * HEIGHT);
		//if (inner_vars.first_col <= (int)round(p[1].x + section_y_progress * diff_21.x))
		//	inner_vars.direct_x = 1;
		//else
		//	inner_vars.direct_x = -1;
		int	len_x = last_col - inner_vars.first_col + inner_vars.direct_x;
		inner_vars.abs_len_x = abs(len_x);
		inner_vars.len_x = (double)len_x;
		inner_loop(inner_vars);
		cur_y_lf += 1.0f;
		row_index =  (int)round(cur_y_lf);
		total_y_progress += total_y_progress_per_step;
		section_y_progress += section_y_progress_per_step;
	}
}




