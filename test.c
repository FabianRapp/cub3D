#include <stdio.h>


int	main(void)
{
	for (int8_t i = 0; i < 3; i++)
	{
		for (int8_t j = i + 1; j < 3; j++)
		{
			printf("i: %d j: %d result((i ^ 3) & (j ^ 3)): %d\n", i, j, (i ^ 3) & (j ^ 3));
		}
	//	printf("i: %d nb1: %d nb2: %d\n", i, (i ^ 3) & 1, (i ^ 3) & 2);
	}
}
