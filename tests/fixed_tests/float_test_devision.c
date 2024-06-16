#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //devision (double / double)
	ff_test_loop(double_test_devision, "double devision (double / double)", 0);
	return (0);
}