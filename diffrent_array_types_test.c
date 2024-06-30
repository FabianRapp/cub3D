#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

typedef uint32_t	t_arr_val;

#define CACHE_LINE_SIZE (64)
// make this values hardcoded later
#define BLOCK_SIZE ((uint8_t)(CACHE_LINE_SIZE / sizeof(t_arr_val)))
#define BLOCK_WIDTH ((uint8_t)floor(log2(BLOCK_SIZE)))
#define BLOCK_HEIGHT ((uint8_t)floor(log2(BLOCK_SIZE)))

//arr is the same allocation as to_free but potentially advanded
// for alignment
typedef struct s_blocked_arr
{
	uint32_t	base_size;
	uint32_t	base_width;
	uint32_t	base_height;
	uint32_t	blocks_per_row;
	uint32_t	blocks_per_col;
	t_arr_val	*arr;
	void		*to_free;
}	t_blocked_arr;

bool	is_power2(uint32_t nb)
{
	return (ceil(log2(nb)) == floor(log2(nb)));
}

void	print_arr_row_major(t_arr_val *arr, uint32_t width, uint32_t height)
{
	printf("row major:\n");
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			uint32_t index = x + y * width;
			//if (!(index % (CACHE_LINE_SIZE / (sizeof *arr))))
			//	printf("|");
			printf("%u\t", arr[index]);
		}
		printf("\n");
	}
}

void	print_arr_cache_lines(t_arr_val *arr, uint32_t width, uint32_t height)
{
	printf("cache lines:\n");
	printf("|\t");
	for (uint32_t index = 0; index < width * height; index++)
	{
		if (index && !(index % (CACHE_LINE_SIZE / (sizeof *arr))))
			printf("|\n|\t");
		printf("%u\t", arr[index]);
	}
	printf("|\n");
}

void	print_blocked_arr_with_xy(t_blocked_arr blocked)
{
	printf("indexing blocked array with x/y (should be same printout as printing base base array row major)\n");
	for (int y = 0; y < blocked.base_height; y++)
	{
		for (int x = 0; x < blocked.base_width; x++)
		{
			uint32_t	base_total_index = x + y * blocked.base_width;
			uint32_t	index_in_block = x % BLOCK_WIDTH + (y % BLOCK_HEIGHT) * BLOCK_WIDTH;
			uint32_t	block_col = x / BLOCK_WIDTH;
			uint32_t	block_row = y / BLOCK_HEIGHT;
			uint32_t	block_nb = block_col + block_row * blocked.blocks_per_row;
			printf("%u\t", blocked.arr[block_nb * BLOCK_SIZE + index_in_block]);
		}
		printf("\n");
	}
}

// returns example row-major array
t_arr_val	*generate_example_ordered(uint32_t width, uint32_t height)
{
	t_arr_val	*arr = malloc(sizeof(t_arr_val) * width * height);
	assert(arr);
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			arr[x + y * width] = 2 * x + y * width;
		}
	}
	return (arr);
}

// returns example row-major array
t_arr_val	*generate_example_random(uint32_t width, uint32_t height)
{
	t_arr_val	*arr = malloc(sizeof(t_arr_val) * width * height);
	assert(arr);
	for (uint32_t y = 0; y < height; y++)
	{
		for (uint32_t x = 0; x < width; x++)
		{
			arr[x + y * width] = rand();
		}
	}
	return (arr);
}


t_blocked_arr create_blocked_arr(t_arr_val *base_arr, uint32_t base_width, uint32_t base_height)
{
	t_blocked_arr	blocked;

	blocked.base_size = base_height * base_width;
	blocked.base_width = base_width;
	blocked.base_height = base_height;
	blocked.blocks_per_row = (uint32_t)ceil((double)base_width / BLOCK_WIDTH);
	blocked.blocks_per_col = (uint32_t)ceil((double)base_height / BLOCK_HEIGHT);
	uint32_t	block_count = blocked.blocks_per_row * blocked.blocks_per_col;
	blocked.to_free = calloc(block_count * BLOCK_SIZE + CACHE_LINE_SIZE, sizeof *base_arr);
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

int	main(void)
{
	assert(is_power2(BLOCK_SIZE));
	srand(time(NULL));
	t_blocked_arr	blocked;
	t_arr_val		*base_arr;
	uint32_t		base_width = 307;
	uint32_t		base_height = 201;
	base_arr = generate_example_ordered(base_width, base_height);
	blocked = create_blocked_arr(base_arr, base_width, base_height);
	printf("base_width: %d, base_height: %d\n", base_width, base_height);

	//printf("base array:\n");
	//print_arr_row_major(base_arr, base_width, base_height);
	//printf("converted to blocked array:\n");
	//print_blocked_arr_with_xy(blocked);
	print_arr_cache_lines(blocked.arr, blocked.blocks_per_row * BLOCK_WIDTH, blocked.blocks_per_col * BLOCK_HEIGHT);
	free(base_arr);
	free(blocked.to_free);
	return (0);
}
