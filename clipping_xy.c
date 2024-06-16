/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clipping_xy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:27:09 by frapp             #+#    #+#             */
/*   Updated: 2024/05/26 03:09:05 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

t_triangle *clipped_g;

t_triangle input;

#define FLAG00 0b00
#define FLAG01 0b01
#define FLAG10 0b10
#define FLAG11 0b11

int count;

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

void	ft_put_binary(int64_t nb)
{
	int	i;

	i = 10;
	while (i >= 0)
	{
		printf("%llu", ((1ULL << i) & nb) >> i);
		if (!(i % 4))
			printf(".");
		i--;
	}
	printf("\n");
}

// returns true if the elemnt in clipped[] is empty
bool	is_empty(t_triangle *clipped)
{
	if (clipped->normal.x == 0.0f && clipped->normal.y == 0.0f && clipped->normal.z == 0.0f)
		return (true);
	return (false);
}

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

bool	index_is_used(t_index_usage usage, int index)
{
	return(usage & (1ULL << index));
}

//00 -> left
//10 -> top
//01 -> right
//11 -> bot
void fast_line_intersect(const t_clipping_para para, t_vec3 p1, t_vec3 *p2)
{
	float progress;
	float dist_x = p2->x - p1.x;
	float dist_y = p2->y - p1.y;
	//assume(!((zero_f(dist_x) && !base_flags[1]) || (zero_f(dist_y) && base_flags[1])));
	assume(!((zero_f(dist_x) && (para.split.right || para.split.left)) || (zero_f(dist_y) && (para.split.top || para.split.bot))));
	//if (base_flags[0])
	if (para.split.right || para.split.bot)
	{
		//if (base_flags[1])
		if (para.split.bot)
			progress = (HEIGHT - 1.0f - p1.y) / dist_y;
		else
			progress = (WIDTH - 1.0f - p1.x) / dist_x;
	}
	else
	{
		//if (base_flags[1])
		if (para.split.top)
			progress = -p1.y / dist_y;
		else
			progress = -p1.x / dist_x;
	}
	p2->x = p1.x + dist_x * progress;
	p2->y = p1.y + dist_y * progress;

	if (p2->x < 0)
		p2->x = 0;
	else if (p2->x >= WIDTH)
		p2->x = WIDTH - 1;
	if (p2->y < 0)
		p2->y = 0;
	else if (p2->y >= HEIGHT)
		p2->y = HEIGHT - 1;

	p2->u = p1.u + (p2->u - p1.u) * progress;
	p2->v = p1.v + (p2->v - p1.v) * progress;
}
//00 -> left
//10 -> top
//01 -> right
//11 -> bot

void	clipping_xy(t_triangle *clipped, t_index_usage *used_indexes, const t_clipping_para para, int8_t cur_index)
{
	int8_t				i;
	int8_t				inside_points;
	int8_t				inside_index[3];

	inside_points = 0;
	i = 0;
	while (i < 3)
	{
		if (clipped[cur_index].p[i].x <= 0.0f && clipped[cur_index].p[i].x >= -0.0001f)
			clipped[cur_index].p[i].x = 0.0f;
		if (clipped[cur_index].p[i].y <= 0.0f && clipped[cur_index].p[i].y >= -0.0001f)
			clipped[cur_index].p[i].y = 0.0f;
		//if (!flags[0] && !flags[1] && clipped[cur_index].p[i].x >= 0.0f)
		if (para.split.left && clipped[cur_index].p[i].x >= 0.0f)
			inside_index[inside_points++] = i;
		//else if (!flags[0] && flags[1] && clipped[cur_index].p[i].y >= 0.0f)
		else if (para.split.top && clipped[cur_index].p[i].y >= 0.0f)
			inside_index[inside_points++] = i;
		//else if (flags[0] && !flags[1] && clipped[cur_index].p[i].x < (float)WIDTH)
		else if (para.split.right && clipped[cur_index].p[i].x < (float)WIDTH)
			inside_index[inside_points++] = i;
		//else if (flags[0] && flags[1] &&clipped[cur_index].p[i].y < (float)HEIGHT)
		else if (para.split.bot && clipped[cur_index].p[i].y < (float)HEIGHT)
		{
			inside_index[inside_points++] = i;
		}
		i++;
	}
	if (inside_points == 0)
	{
		set_index_usage(used_indexes, cur_index, 0);
		count--;
		return ;
	}
	if (inside_points == 3)
	{
		return ;
	}
	if (inside_points == 1)
	{
		set_index_usage(used_indexes, cur_index, 1);
		int8_t	outside_index1 = (inside_index[0] ^ 3) & 1; //xd
		int8_t	outside_index2 = (inside_index[0] ^ 3) & 2;
		fast_line_intersect(para, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index1);
		fast_line_intersect(para, clipped[cur_index].p[inside_index[0]], clipped[cur_index].p + outside_index2);
		clipped[cur_index].col = RED;
		return ;
	}
	count++;
	int8_t second_index;
	set_index_usage(used_indexes, cur_index, 1);
	second_index = get_free_index(used_indexes);
	set_index_usage(used_indexes, second_index, true);
	clipped[second_index] = clipped[cur_index];
	int8_t	outside_index = (inside_index[0] ^ 3) & (inside_index[1] ^ 3);
	fast_line_intersect(para, clipped[cur_index].p[inside_index[0]], (clipped[cur_index].p) + outside_index);
	t_vec3 input_fn_left = clipped[second_index].p[inside_index[1]];
	t_vec3 input_fn_right = clipped[second_index].p[outside_index];
	fast_line_intersect(para, clipped[second_index].p[inside_index[1]], (clipped[second_index].p) + outside_index);
	t_vec3	output_fn = clipped[second_index].p[outside_index];
	clipped[second_index].p[inside_index[0]] = clipped[cur_index].p[outside_index];
	clipped[cur_index].col = PINK;
	clipped[cur_index].col = DARK_GREY;
}

int8_t	fix_clipped_arr(t_triangle *clipped, t_index_usage used_indexes)
{
	int8_t	first_empty_index;
	int8_t	last_used_index;

	int		swap_count = 0;
	if (!used_indexes)
		return (0);
	first_empty_index = get_free_index(&used_indexes);
	last_used_index = get_unset_last_used_index(&used_indexes);
	if (last_used_index == -1)
		return (0);
	assume(last_used_index != first_empty_index);
	while (last_used_index > first_empty_index)
	{
		assume(last_used_index != first_empty_index);
		clipped[first_empty_index] = clipped[last_used_index];
		set_index_usage(&used_indexes, first_empty_index, true);
		swap_count++;
		//clipped[last_used_index].p[0].x = 11111;
		first_empty_index = get_free_index(&used_indexes);
		last_used_index = get_unset_last_used_index(&used_indexes);
		//printf("first_empty_index: %d\nlast_used_index: %d\n", first_empty_index, last_used_index);
		assume(last_used_index != -1);
	}
	set_index_usage(&used_indexes, last_used_index, true);
	assume(last_used_index != first_empty_index);
	
	if (count != first_empty_index)
	{
		fprintf(stderr, "Wrong count in fixed_clipped_arr, expeceted: %d actual: %d\n", count, first_empty_index +1);
		assume(count != first_empty_index);
	}
	return (count);
}

//00 -> left
//10 -> top
//01 -> right
//11 -> bot

// returns the new count
int8_t	call_clipping_xy(t_triangle *clipped)
{
	//int8_t			clipping_flags[2];
	t_clipping_para	para;
	int8_t			cur_index;
	t_index_usage	used_indexes;
	t_index_usage	used_indexes_old;

	input = clipped[0];
	clipped_g = clipped;
	para.all = 0;
	para.split.left = 1;
	//clipping_flags[0] = 0;
	//clipping_flags[1] = 0;
	cur_index = 0;
	used_indexes = 1ULL;
	count = 1;
	//clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
	clipping_xy(clipped, &used_indexes, para, cur_index);
	if (!used_indexes)
		return (0);
	para.all = 0;
	para.split.top = 1;
	//clipping_flags[0] = 0;
	//clipping_flags[1] = 1;
	used_indexes_old = used_indexes;
	cur_index = get_unset_used_index(&used_indexes_old);
	//while (cur_index != -1)
	int old_count = count;
	for (int i =0; i < old_count; i++)
	{
		assume(cur_index != -1);
		//clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		clipping_xy(clipped, &used_indexes, para, cur_index);
		if (!used_indexes)
			return (0);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	para.all = 0;
	para.split.right = 1;
	//clipping_flags[0] = 1;
	//clipping_flags[1] = 0;
	used_indexes_old = used_indexes;
	cur_index = get_unset_used_index(&used_indexes_old);
	while (cur_index != -1)
	{
		//clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		clipping_xy(clipped, &used_indexes, para, cur_index);
		if (!used_indexes)
			return (0);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	para.all = 0;
	para.split.bot = 1;
	//clipping_flags[0] = 1;
	//clipping_flags[1] = 1;
	used_indexes_old = used_indexes;
	cur_index = get_unset_used_index(&used_indexes_old);
	while (cur_index != -1)
	{
		//clipping_xy(clipped, &used_indexes, clipping_flags, cur_index);
		clipping_xy(clipped, &used_indexes, para, cur_index);
		if (!used_indexes)
			return (0);
		cur_index = get_unset_used_index(&used_indexes_old);
	}
	return (fix_clipped_arr(clipped, used_indexes));
}
