#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //devision (float / float)
	ff_test_loop(fixed_test_devision, "fixed devision (float / float)", 0);
	return (0);
}