#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	//devision (float / int)
	ff_test_loop(fixed_test_devision_fi, "fixed devision (float / int)", 0);
	return (0);
}
