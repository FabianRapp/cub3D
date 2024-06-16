#include <fcntl.h>
#include <sys/time.h>

#define ABS_MACRO(x) ((x) < 0 ? -(x) : (x))
#define LOOP_ITER_NEGATIVE 1000
#define LOOP_ITER_POSITIVE 1000
#define CALC_PER_ITER 20

#define LOG_FILE "time_logs.log"



// #define FT_INLINE_TEST_FT

struct timeval	time_diff(struct timeval start, struct timeval end);
struct timeval	timer(char *msg);
void	log_diff(struct timeval diff, char *title);
double	ref_int_int_add(double a, double b);
double	double_test_additon(double a, double b);
double	fixed_test_additon(double a, double b);
double	double_test_subtraction(double a, double b);
double	fixed_test_devision(double a, double b);
double	fixed_test_subtraction(double a, double b);
double	double_test_multiplication(double a, double b);
double	fixed_test_multiplication(double a, double b);
double	double_test_devision(double a, double b);
double	double_test_devision_fi(double a, double b);
double	fixed_test_devision_fi(double a, double b);
double	double_test_additon_fi(double a, double b);
double	fixed_test_additon_fi(double a, double b);
double	double_linear_comb_test_fn(double a, double b);
double	fixed_linear_comb_test_fn(double a, double b);
int64_t	ff_test_loop(double f(double, double), char *timer_msg, int64_t time_sofar);