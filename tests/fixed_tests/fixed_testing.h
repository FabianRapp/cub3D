#include <fcntl.h>
#include <sys/time.h>
struct timeval	time_diff(struct timeval start, struct timeval end);
struct timeval	timer(char *msg);
void	log_diff(struct timeval diff, char *title);
float	float_test_additon(float a, float b);
float	fixed_test_additon(float a, float b);
float	float_test_subtraction(float a, float b);
float	fixed_test_devision(float a, float b);
float	fixed_test_subtraction(float a, float b);
float	float_test_multiplication(float a, float b);
float	fixed_test_multiplication(float a, float b);
float	float_test_devision(float a, float b);
float	float_test_devision_fi(float a, float b);
float	fixed_test_devision_fi(float a, float b);
float	float_test_additon_fi(float a, float b);
float	fixed_test_additon_fi(float a, float b);
float	float_linear_comb_test_fn(float a, float b);
float	fixed_linear_comb_test_fn(float a, float b);
int64_t	ff_test_loop(float f(float, float), char *timer_msg, int64_t time_sofar);