#include <cub3D.h>
#include "fixed_testing.h"

int main(void)
{
	//additon (float / int)
	ff_test_loop(float_test_additon_fi, "float additon (float + int)", 0);
	return (0);
}