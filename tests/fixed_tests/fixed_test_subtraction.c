#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	//subraction (float - float)
	ff_test_loop(fixed_test_subtraction, "fixed subtraction (float - float)", 0);
	return (0);
}