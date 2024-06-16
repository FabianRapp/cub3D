
#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	//additon (double / int)
	ff_test_loop(fixed_test_additon_fi, "fixed additon (double + int)", 0);
	return (0);
}