#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //multiplication (double * double)
	ff_test_loop(fixed_test_multiplication, "fixed multiplication (double * double)", 0);
	return (0);
}