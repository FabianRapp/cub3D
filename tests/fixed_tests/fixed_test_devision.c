#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //devision (double / double)
	ff_test_loop(fixed_test_devision, "fixed devision (double / double)", 0);
	return (0);
}