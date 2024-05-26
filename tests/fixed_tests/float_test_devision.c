#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //devision (float / float)
	ff_test_loop(float_test_devision, "float devision (float / float)", 0);
	return (0);
}