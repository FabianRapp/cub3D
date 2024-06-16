#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	//subraction (double - double)
	ff_test_loop(double_test_subtraction, "double subtraction (double - double)", 0);
	return (0);
}