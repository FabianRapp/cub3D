#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //additon (float + float)
	ff_test_loop(fixed_test_additon, "fixed additon (float + float)", 0);
	return (0);
}