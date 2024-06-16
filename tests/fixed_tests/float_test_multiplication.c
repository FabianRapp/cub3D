#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //multiplication (double * double)
	ff_test_loop(double_test_multiplication, "double multiplication (double * double)", 0);
	return (0);
}