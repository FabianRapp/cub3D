#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	//devision (double / int)
	ff_test_loop(fixed_test_devision_fi, "fixed devision (double / int)", 0);
	return (0);
}
