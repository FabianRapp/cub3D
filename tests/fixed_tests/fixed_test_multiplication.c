#include <cub3D.h>
#include "fixed_testing.h"

int main(void)
{
	// //multiplication (float * float)
	ff_test_loop(fixed_test_multiplication, "fixed multiplication (float * float)", 0);
	return (0);
}