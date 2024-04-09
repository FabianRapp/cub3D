#include <cub3D.h>
#include "fixed_testing.h"

int main(void)
{
	//subraction (float - float)
	ff_test_loop(float_test_subtraction, "float subtraction (float - float)", 0);
	return (0);
}