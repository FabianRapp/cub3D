#include <cub3D.h>
#include "fixed_testing.h"

int main(void)
{
	//devision (float / int)
	ff_test_loop(float_test_devision_fi, "float devision (float / int)", 0);
	return (0);
}