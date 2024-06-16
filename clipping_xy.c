/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/06/16 06:45:13 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

typedef union u_clipping_para
{
	int16_t	all;
	struct
	{
		int8_t	bot		:2;
		int8_t	top		:2;
		int8_t	left	:2;
		int8_t	right	:2;
	}		split;
}	t_clipping_para;

int8_t	get_free_index(t_index_usage *const usage)
{
	int8_t		i;
	t_index_usage	local;

	i = 0;
	local = *usage;
	while (i < 64)
	{
		if ((local & (1ULL << i)) == 0)
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

int8_t	get_unset_last_used_index(t_index_usage *const usage)
{
	int8_t			i;
	t_index_usage	local;

	i = 63;
	local = *usage;
	while (i >= 0)
	{
		if (local & (1ULL << i))
		{
			*usage ^= 1ULL << i;
			return (i);
		}
		i--;
	}
	return (-1);
}

int8_t	get_unset_used_index(t_index_usage *usage)
{
	int8_t		i;

	i = 0;
	while (i < 64)
	{
		if (((*usage) & (1ULL << i)))
		{
			*usage &= ~(1ULL << i);
			return (i);
		}
		i++;
	}
	return (-1);
}

void	set_index_usage(t_index_usage *usage, int8_t index, int8_t state)
{
	if (state)
		*usage |= (1ULL << index);
	else
	{
		*usage &= ~(1ULL << index);
	}
}

double	clamplf(double val, double min, double max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}

// call order:
//left
//top
//right
//bot
// todo: uv values need same bounds security
void fast_line_intersect(const t_clipping_para para, t_vec3 p1, t_vec3 *p2)
{
	double progress;
	double dist_x = p2->x - p1.x;
	double dist_y = p2->y - p1.y;
	assume(!((zero_f(dist_x) && (para.split.right || para.split.left)) || (zero_f(dist_y) && (para.split.top || para.split.bot))));
	if (para.split.bot)
	{
		progress = (HEIGHT - 1.0 - p1.y) / dist_y;
		p2->y = HEIGHT - 1.1;
		p2->x = p1.x + dist_x * progress;
		p2->x = clamplf(p2->x, 0.0f, WIDTH - 1.1);
	}
	else if (para.split.right)
	{
		progress = (WIDTH - 1.0f - p1.x) / dist_x;
		p2->x = WIDTH - 4.0f;
		p2->y = p1.y + dist_y * progress;
		p2->y = clamplf(p2->y, 0, HEIGHT - 1.1);
	}
	else if (para.split.top)
	{
		progress = -p1.y / dist_y;
		p2->y = 0;
		p2->x = p1.x + dist_x * progress;
		p2->x = clamplf(p2->x, 0.0f, WIDTH - 1.1);
	}
	else if (para.split.left)
	{
		progress = -p1.x / dist_x;
		p2->x = 3;
		p2->y = p1.y + dist_y * progress;
		p2->y = clamplf(p2->y, 0, HEIGHT - 1.1);
	}
	else
		assume(0);
	//p2->x = p1.x + dist_x * progress;
	//p2->y = p1.y + dist_y * progress;

	//if (p2->x < 0)
	//	p2->x = 0;
	//else if (p2->x >= WIDTH)
	//	p2->x = WIDTH - 1;
	//if (p2->y < 0)
	//	p2->y = 0;
	//else if (p2->y >= HEIGHT)
	//	p2->y = HEIGHT - 1;

	p2->u = p1.u + (p2->u - p1.u) * progress;
	p2->v = p1.v + (p2->v - p1.v) * progress;
}

int8_t	count_inside_points(t_triangle *clipped, int8_t cur_index, t_clipping_para para, int8_t inside_index[3])
{
	int8_t				inside_points;
	int					i;

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (clipped[cur_index].p[i].x <= 0.0f && clipped[cur_index].p[i].x >= -0.0001f)
			clipped[cur_index].p[i].x = 0.0f;
		if (clipped[cur_index].p[i].y <= 0.0f && clipped[cur_index].p[i].y >= -0.0001f)
			clipped[cur_index].p[i].y = 0.0f;
		if (para.split.left && clipped[cur_index].p[i].x >= 0.0f)
			inside_index[inside_points++] = i;
		else if (para.split.top && clipped[cur_index].p[i].y >= 0.0f)
			inside_index[inside_points++] = i;
		else if (para.split.right && clipped[cur_index].p[i].x < (double)WIDTH)
			inside_index[inside_points++] = i;
		else if (para.split.bot && clipped[cur_index].p[i].y < (double)HEIGHT)
			inside_index[inside_points++] = i;
		i++;
	}
	return (inside_points);
}

// returns the amount of additional/removed triangles
int	clipping_xy(t_triangle *clipped, t_index_usage *used_indexes, const t_clipping_para para, int8_t cur_index)
{
	int8_t				inside_points;
	int8_t				inside_index[3];

	inside_points = count_inside_points(clipped, cur_index, para, inside_index);
	if (inside_points == 0)
	{
		set_index_usage(used_indexes, cur_index, 0);
		return (-1);
	}
	if (inside_points == 3)
		return (0);
	if (inside_points == 1)
	{
		set_index_usage(used_indexes, cur_index, 1);
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		fast_line_intersect(para, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index1);
		fast_line_intersect(para, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index2);
		clipped[cur_index].col = RED;
		return (0);;
	}
	//if inside_points == 2
	set_index_usage(used_indexes, cur_index, 1);
	int8_t	second_index = get_free_index(used_indexes);
	set_index_usage(used_indexes, second_index, true);
	clipped[second_index] = clipped[cur_index];
	int8_t	outside_index = 3 - inside_index[0] - inside_index[1];
	fast_line_intersect(para, clipped[cur_index].p[inside_index[0]], (clipped[cur_index].p) + outside_index);
	t_vec3 input_fn_left = clipped[second_index].p[inside_index[1]];
	t_vec3 input_fn_right = clipped[second_index].p[outside_index];
	fast_line_intersect(para, clipped[second_index].p[inside_index[1]], (clipped[second_index].p) + outside_index);
	t_vec3	output_fn = clipped[second_index].p[outside_index];
	clipped[second_index].p[inside_index[0]] = clipped[cur_index].p[outside_index];
	clipped[second_index].col = DARK_GREY;
	clipped[cur_index].col = PINK;//this triangle can be buggy
	return (1);
}

int8_t	fix_clipped_arr(t_triangle *clipped, t_index_usage used_indexes)
{
	int8_t	first_empty_index;
	int8_t	last_used_index;

	if (!used_indexes)
		return (0);
	first_empty_index = get_free_index(&used_indexes);
	last_used_index = get_unset_last_used_index(&used_indexes);
	if (last_used_index == -1)
		return (0);
	assume(last_used_index != first_empty_index);
	while (last_used_index > first_empty_index)
	{
		clipped[first_empty_index] = clipped[last_used_index];
		set_index_usage(&used_indexes, first_empty_index, true);
		first_empty_index = get_free_index(&used_indexes);
		last_used_index = get_unset_last_used_index(&used_indexes);
		//printf("first_empty_index: %d\nlast_used_index: %d\n", first_empty_index, last_used_index);
		assume(last_used_index != -1);
	}
	set_index_usage(&used_indexes, last_used_index, true);
	assume(last_used_index != first_empty_index);
	return (first_empty_index);
}

int	clipp_loop(int count, t_triangle *clipped, t_clipping_para para, t_index_usage *used_indexes_old, t_index_usage *used_indexes)
{
	int8_t	cur_index;

	cur_index = get_unset_used_index(used_indexes_old);
	int old_count = count;
	while (cur_index != -1)
	{
		count += clipping_xy(clipped, used_indexes, para, cur_index);
		if (!count)
			return (0);
		cur_index = get_unset_used_index(used_indexes_old);
	}
	return (count);
}

// returns the new count
int8_t	call_clipping_xy(t_triangle *clipped)
{
	//int8_t			clipping_flags[2];
	t_clipping_para	para;
	t_index_usage	used_indexes;
	t_index_usage	used_indexes_old;
	int				count = 1;

	para.all = 0;
	para.split.left = 1;
	used_indexes = 1ULL;
	count += clipping_xy(clipped, &used_indexes, para, 0);
	if (!count)
		return(0);
	para.all = 0;
	para.split.top = 1;
	used_indexes_old = used_indexes;
	count = clipp_loop(count, clipped, para, &used_indexes_old, &used_indexes);
	if (!count)
		return(0);
	para.all = 0;
	para.split.right = 1;
	used_indexes_old = used_indexes;
	count = clipp_loop(count, clipped, para, &used_indexes_old, &used_indexes);
	if (!count)
		return(0);
	para.all = 0;
	para.split.bot = 1;
	used_indexes_old = used_indexes;
	count = clipp_loop(count, clipped, para, &used_indexes_old, &used_indexes);
	if (!count)
		return(0);
	assume (count == fix_clipped_arr(clipped, used_indexes));
	return (count);
}
