
#include "../../../includes/cub3D.h"
#include "fixed_testing.h"

// double	double_linear_comb_test_fn(double a, double b)
// {
// 	double	x;
// 	double	y;

// 	x = a * 2.5f + b * 3.5f;
// 	y = a * 4.5f - b * 1.5f;
// 	return (x + y);
// }

// double	fixed_linear_comb_test_fn(double a, double b)
// {
// 	t_fixed fa;
// 	t_fixed fb;
// 	t_fixed fc;
// 	t_fixed fd;
// 	t_fixed fe;

// 	fa = double_to_fixed(a);
// 	fb = double_to_fixed(b);
// 	fc = fixed_mult(fa, double_to_fixed(2.5f)) + fixed_mult(fb, double_to_fixed(3.5f));
// 	fd = fixed_mult(fa, double_to_fixed(4.5f)) - fixed_mult(fb, double_to_fixed(1.5f));
// 	fe = fc + fd;
// 	return fixed_to_double(fe);
// }

static inline t_fixed __attribute__((always_inline))	double_to_fixed1(double nb)
{
	return ((t_fixed)(nb * F_FIXED_MULTI));
}

static inline t_fixed __attribute__((always_inline))	fixed_mult1(t_fixed a, t_fixed b)
{
	return (t_fixed)((a * b) >> FRACTION_BITS);
}

// for performence reason abc(a) with a huge value is not supported
static inline t_fixed __attribute__((always_inline))	fixed_dev1(t_fixed a, t_fixed b)
{
	if (b)
		return ((a << FRACTION_BITS) / b);
	return (INT_MAX);
}

static inline double __attribute__((always_inline))	fixed_to_double1(t_fixed nb)
{
	return (((double)nb) / F_FIXED_MULTI);
}

struct timeval	time_diff(struct timeval start, struct timeval end)
{
	struct timeval	diff;

	diff.tv_sec = end.tv_sec - start.tv_sec;
	if (end.tv_usec >= start.tv_usec)
		diff.tv_usec = end.tv_usec - start.tv_usec;
	else
	{
		diff.tv_sec--;
		diff.tv_usec = 1000000 - start.tv_usec + end.tv_usec;
	}
	return (diff);
}

struct timeval	timer(char *msg)
{
	static struct timeval	start_time;
	struct timeval			fin_time;
	static bool				first = true;
	struct timeval			diff;

	gettimeofday(&fin_time, NULL);
	if (!first)
	{
		diff = time_diff(start_time, fin_time);
		//if (msg)
			//fprintf(stderr, "%ssecond: %ld\nadditonal microseconds: %d\n", msg, diff.tv_sec, diff.tv_usec);
		first = true;
		return fin_time;
	}
	first = false;
	gettimeofday(&start_time, NULL);
	return start_time;
}

void	log_diff(struct timeval diff, char *title)
{
	FILE *fd = fopen(LOG_FILE, "a");
	int64_t	micro_diff = diff.tv_sec * 1000000 + diff.tv_usec;
	fprintf(fd, "%s: %lld\n", title, micro_diff);
	fprintf(stderr, "%s: %lld\n", title, micro_diff);
	fclose(fd);
}

double	ref_int_int_add(double a, double b)
{
	int c = (int)b;

	for (int i = 0; i < CALC_PER_ITER; i++)
	{
		c = c + i;
	}
	return (a);
}

double	double_test_additon(double a, double b)
{
	for (int i = 0; i < CALC_PER_ITER; i++)
	{
		a = a + b;
	}
	return (a);
}

double	fixed_test_additon(double a, double b)
{
	t_fixed	fa;
	t_fixed	fb;

	#ifdef FT_INLINE_TEST_FT
		fa = double_to_fixed1(a);
		fb = double_to_fixed1(b);
	#else
		fa = double_to_fixed(a);
		fb = double_to_fixed(b);
	#endif
	for (int i = 0; i < CALC_PER_ITER; i++)
	{
		fa = fa + fb;
	}
	#ifdef FT_INLINE_TEST_FT
		return (fixed_to_double1(fa));
	#else
		return (fixed_to_double(fa));
	#endif
}

double	double_test_subtraction(double a, double b)
{
	for (int i = 0; i < CALC_PER_ITER; i++)
	{
		a = a - b;
	}
	return (a);
}

double	fixed_test_subtraction(double a, double b)
{
	t_fixed	fa;
	t_fixed	fb;

	#ifdef FT_INLINE_TEST_FT
		fa = double_to_fixed1(a);
		fb = double_to_fixed1(b);
			for (int i = 0; i < CALC_PER_ITER; i++)
		{
			fa = fa - fb;
		}
		return (fixed_to_double1(fa));
	#else
		fa = double_to_fixed(a);
		fb = double_to_fixed(b);
			for (int i = 0; i < CALC_PER_ITER; i++)
		{
			fa = fa - fb;
		}
		return (fixed_to_double(fa));
	#endif
}

double	double_test_multiplication(double a, double b)
{
	for (int i = 0; i < CALC_PER_ITER; i++)
	{
		a = a * b;
	}
	return (a);
}

double	fixed_test_multiplication(double a, double b)
{
	t_fixed	fa;
	t_fixed	fb;

	#ifdef FT_INLINE_TEST_FT
		fa = double_to_fixed1(a);
		fb = double_to_fixed1(b);
			for (int i = 0; i < CALC_PER_ITER; i++)
		{
			fa = fixed_mult1(fa, fb);
		}
		return (fixed_to_double1(fa));
	#else
		fa = double_to_fixed(a);
		fb = double_to_fixed(b);
			for (int i = 0; i < CALC_PER_ITER; i++)
		{
			fa = fixed_mult(fa, fb);
		}
		return (fixed_to_double(fa));
	#endif
}

double	double_test_devision(double a, double b)
{
	for (int i = 0; i < CALC_PER_ITER; i++)
	{
		a = a / b;
	}
	return (a);
}

double	fixed_test_devision(double a, double b)
{
	t_fixed	fa;
	t_fixed	fb;

	#ifdef FT_INLINE_TEST_FT
		fa = double_to_fixed1(a);
		fb = double_to_fixed1(b);
			for (int i = 0; i < CALC_PER_ITER; i++)
		{
			fa = fixed_dev1(fa, fb);
		}
		return (fixed_to_double1(fa));
	#else
		fa = double_to_fixed(a);
		fb = double_to_fixed(b);
			for (int i = 0; i < CALC_PER_ITER; i++)
		{
			fa = fixed_dev(fa, fb);
		}
		return (fixed_to_double(fa));
	#endif
}

double	double_test_devision_fi(double a, double b)
{
	(void)b;
	for (int i = 1; i < CALC_PER_ITER + 1; i++)
	{
		a = a / i;
	}
	return (a);
}

double	fixed_test_devision_fi(double a, double b)
{
	t_fixed	fa;

	(void)b;
	#ifdef FT_INLINE_TEST_FT
		fa = double_to_fixed1(a);
			for (int i = 1; i < CALC_PER_ITER + 1; i++)
		{
			fa = fa / i;
		}
		return (fixed_to_double1(fa));
	#else
		fa = double_to_fixed(a);
			for (int i = 1; i < CALC_PER_ITER + 1; i++)
		{
			fa = fa / i;
		}
		return (fixed_to_double(fa));
	#endif
}

double	double_test_additon_fi(double a, double b)
{
	(void)b;
	for (int i = 1; i < CALC_PER_ITER + 1; i++)
	{
		a = a + i;
	}
	return (a);
}

double	fixed_test_additon_fi(double a, double b)
{
	t_fixed	fa;

	(void)b;
	#ifdef FT_INLINE_TEST_FT
		fa = double_to_fixed1(a);
			for (int i = 1; i < CALC_PER_ITER + 1; i++)
		{
			fa = fa + i;
		}
		return (fixed_to_double1(fa));
	#else
		fa = double_to_fixed(a);
			for (int i = 1; i < CALC_PER_ITER + 1; i++)
		{
			fa = fa + i;
		}
		return (fixed_to_double(fa));
	#endif
}

int64_t	ff_test_loop(double f(double, double), char *timer_msg, int64_t time_sofar)
{
	struct timeval	start_time;
	struct timeval	end_time;
	struct timeval	diff;
	const double		goal = LOOP_ITER_POSITIVE;
	const double		start = -LOOP_ITER_NEGATIVE;
	double			a = start;
	double			b;
	double			return_val;

	usleep(100000);
	int64_t	arr[10000];
	double	arr2[10000];
	for (int i = 0; i < 10000; i++)
	{
		arr[i] = i;
		arr2[i] = i;
	}
	start_time = timer(NULL);
	while (a < goal)
	{
		b = start;
		while (b < goal)
		{
			return_val = f(a, b);
			b++;
		}
		a++;
	}
	end_time = timer(timer_msg);
	diff = time_diff(start_time, end_time);
	log_diff(diff, timer_msg);
	time_sofar += diff.tv_sec * 1000000 + diff.tv_usec;
	(void)return_val;
	return (time_sofar);
}
