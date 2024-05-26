/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <fabi@student.42.fr>                 +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:41:23 by frapp             #+#    #+#             */
/*   Updated: 2024/05/26 03:14:45 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"
#include "includes/menu.h"

bool	arr_append_test(void)
{
	int	*arr = NULL;
	int	size = 0;

	for (int i = -4; i < 5; i++)
	{
		if (!arr_append((void **)(&arr), &i, sizeof(int), size))
			printf("arr_append returned false (old size: %d)\n", size);
		if (!arr)
			printf("arr is NULL!\n");
		size++;
	}
	printf("arr after arr_append_test:\n");
	for (int i =0; i < size; i++)
	{
		printf("arr[%d]: %d\n", i, arr[i]);
	}
	free(arr);
}
