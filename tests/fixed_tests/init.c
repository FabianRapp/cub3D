#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	FILE *fd = fopen(LOG_FILE, "a");

	fprintf(fd, "total iterations: %d with %d calcs each\n", ft_powint(LOOP_ITER_NEGATIVE + LOOP_ITER_POSITIVE, 2), CALC_PER_ITER);
	#ifdef FT_INLINE_TEST_FT
		fprintf(fd, "using inline\n");
	#else
		fprintf(fd, "not using inline\n");
	#endif
	fclose(fd);
	return (0);
}
