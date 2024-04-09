
#include <cub3D.h>
#include "fixed_testing.h"

int main(void)
{
	//additon (float / int)
	ff_test_loop(fixed_test_additon_fi, "fixed additon (float / int)", 0);
	return (0);
}