#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //multiplication (float * float)
	ff_test_loop(float_test_multiplication, "float multiplication (float * float)", 0);
	return (0);
}