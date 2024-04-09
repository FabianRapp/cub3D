
#include <cub3D.h>
#include "fixed_testing.h"

int main(void)
{
	int64_t	total_float_time = 0;
	int64_t	total_fixed_time = 0;

	//subraction (float - float)
	total_float_time = ff_test_loop(fixed_test_subtraction, "fixed subtraction (float - float)", total_float_time);
	total_fixed_time = ff_test_loop(float_test_subtraction, "float subtraction (float - float)", total_fixed_time);

	// //additon (float + float)
	total_float_time = ff_test_loop(fixed_test_additon, "fixed additon (float + float)", total_float_time);
	total_fixed_time = ff_test_loop(float_test_additon, "float additon (float + float)", total_fixed_time);

	// //multiplication (float * float)
	total_float_time = ff_test_loop(fixed_test_multiplication, "fixed multiplication (float * float)", total_float_time);
	total_fixed_time = ff_test_loop(float_test_multiplication, "float multiplication (float * float)", total_fixed_time);

	// //devision (float / float)
	total_float_time = ff_test_loop(fixed_test_devision, "fixed devision (float / float)", total_float_time);
	total_fixed_time = ff_test_loop(float_test_devision, "float devision (float / float)", total_fixed_time);

	//devision (float / int)
	total_float_time = ff_test_loop(fixed_test_devision_fi, "fixed devision (float / int)", total_float_time);
	total_fixed_time = ff_test_loop(float_test_devision_fi, "float devision (float / int)", total_fixed_time);

	//additon (float / int)
	total_float_time = ff_test_loop(fixed_test_additon_fi, "fixed additon (float / int)", total_float_time);
	total_fixed_time = ff_test_loop(float_test_additon_fi, "float additon (float / int)", total_fixed_time);

	// if (total_fixed_time > total_float_time)
	// 	printf("total FIXED time was %lld microsec slower!\n", total_fixed_time - total_float_time);
	// else
	// 	printf("total float time was %lld microsec slower!\n", total_float_time - total_fixed_time);
	return (0);
}