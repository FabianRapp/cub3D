#include "includes/cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

t_blocked_arr create_blocked_arr(t_arr_val *base_arr, uint32_t base_width, uint32_t base_height)
{
	t_blocked_arr	blocked;

	blocked.base_size = base_height * base_width;
	blocked.base_width = base_width;
	blocked.base_height = base_height;
	blocked.blocks_per_row = (uint32_t)ceil((double)base_width / BLOCK_WIDTH);
	blocked.blocks_per_col = (uint32_t)ceil((double)base_height / BLOCK_HEIGHT);
	uint32_t	block_count = blocked.blocks_per_row * blocked.blocks_per_col;
	blocked.to_free = ft_calloc(block_count * BLOCK_SIZE + CACHE_LINE_SIZE, sizeof *base_arr);
	assert(blocked.to_free);
	blocked.arr = blocked.to_free;
	while ((uintptr_t)blocked.arr % CACHE_LINE_SIZE)
		blocked.arr++;
	for (uint32_t blocked_row = 0; blocked_row < blocked.blocks_per_col; blocked_row++)
	{
		for (uint32_t blocked_col = 0; blocked_col < blocked.blocks_per_row; blocked_col++)
		{
			uint32_t	cur_block = blocked_row * blocked.blocks_per_row + blocked_col;
			for (uint8_t y = 0; y < BLOCK_HEIGHT; y++)
			{
				for (uint8_t x = 0; x < BLOCK_WIDTH; x++)
				{
					uint32_t	base_x = blocked_col * BLOCK_WIDTH + x;
					if (base_x >= base_width)
						break ;
					uint32_t	base_y = blocked_row * BLOCK_HEIGHT + y;
					if (base_y >= base_height)
						break ;
					blocked.arr[cur_block * BLOCK_SIZE + y * BLOCK_WIDTH + x] = base_arr[base_x + base_y * base_width];
				}
			}
		}
	}
	return (blocked);
}
