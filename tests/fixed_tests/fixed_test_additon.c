#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //additon (double + double)
	ff_test_loop(fixed_test_additon, "fixed additon (double + double)", 0);
	return (0);
}