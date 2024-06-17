#include "../includes/cub3D.h"


void	print_matrix(double m[4][4])
{
	int	i;
	int	j;

	while (i < 4)
	{
		printf("|");
		while (j < 4)
		{
			printf("%lf|", m[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
}

