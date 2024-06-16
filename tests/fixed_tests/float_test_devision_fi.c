#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	//devision (double / int)
	ff_test_loop(double_test_devision_fi, "double devision (double / int)", 0);
	return (0);
}