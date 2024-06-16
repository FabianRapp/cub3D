#include "../../includes/cub3D.h"
#include "fixed_testing.h"

int main(void)
{
	// //devision (double / double)
	ff_test_loop(ref_int_int_add, "REFERENCE (INT + INT with 1 double to int cast)", 0);
	return (0);
}
