
#include <cub3D.h>
#include "fixed_testing.h"

// static inline  t_fixed	float_to_fixed(float nb)
// {
// 	return ((t_fixed)(nb * F_FIXED_MULTI));
// }

// static inline t_fixed	fixed_mult(t_fixed a, t_fixed b)
// {
// 	return (t_fixed)(((int64_t)a * (int64_t)b) >> FRACTION_BITS);
// }

// // for performence reason abc(a) with a huge value is not supported
// static inline t_fixed	fixed_dev(t_fixed a, t_fixed b)
// {
// 	if (b)
// 		return ((a << FRACTION_BITS) / b);
// 	return (INT_MAX);
// }

// static inline float	fixed_to_float(t_fixed nb)
// {
// 	return (((float)nb) / F_FIXED_MULTI);
// }

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
			//printf("%ssecond: %ld\nadditonal microseconds: %d\n", msg, diff.tv_sec, diff.tv_usec);
		first = true;
		return fin_time;
	}
	first = false;
	gettimeofday(&start_time, NULL);
	return start_time;
}


void	log_diff(struct timeval diff, char *title)
{
	FILE *fd = fopen("time_logs", "a");
	int64_t	micro_diff = diff.tv_sec * 1000000 + diff.tv_usec;
	///fprintf(fd, "%s: %ld\n", title, micro_diff);
	printf("%s: %lld\n", title, micro_diff);
	fclose(fd);
}

float	float_test_additon(float a, float b)
{
	a = a + b; //1
	a = a + b; //2
	a = a + b; //3
	a = a + b; //4
	a = a + b; //5
	a = a + b; //6
	a = a + b; //7
	a = a + b; //8
	a = a + b; //9
	a = a + b; //10
	a = a + b; //1
	a = a + b; //2
	a = a + b; //3
	a = a + b; //4
	a = a + b; //5
	a = a + b; //6
	a = a + b; //7
	a = a + b; //8
	a = a + b; //9
	a = a + b; //10
	return (a);
}

float	fixed_test_additon(float a, float b)
{
	t_fixed	fa;
	t_fixed	fb;

	fa = float_to_fixed(a);
	fb = float_to_fixed(b);
	fa = fa + fb;//1
	fa = fa + fb;//2
	fa = fa + fb;//3
	fa = fa + fb;//4
	fa = fa + fb;//5
	fa = fa + fb;//6
	fa = fa + fb;//7
	fa = fa + fb;//8
	fa = fa + fb;//9
	fa = fa + fb;//10
	fa = fa + fb;//1
	fa = fa + fb;//2
	fa = fa + fb;//3
	fa = fa + fb;//4
	fa = fa + fb;//5
	fa = fa + fb;//6
	fa = fa + fb;//7
	fa = fa + fb;//8
	fa = fa + fb;//9
	fa = fa + fb;//10
	return (fixed_to_float(fa));
}

float	float_test_subtraction(float a, float b)
{
	a = a - b; //1
	a = a - b; //2
	a = a - b; //3
	a = a - b; //4
	a = a - b; //5
	a = a - b; //6
	a = a - b; //7
	a = a - b; //8
	a = a - b; //9
	a = a - b; //10
	a = a - b; //1
	a = a - b; //2
	a = a - b; //3
	a = a - b; //4
	a = a - b; //5
	a = a - b; //6
	a = a - b; //7
	a = a - b; //8
	a = a - b; //9
	a = a - b; //10
	return (a);
}


float	fixed_test_subtraction(float a, float b)
{
	t_fixed	fa;
	t_fixed	fb;

	fa = float_to_fixed(a);
	fb = float_to_fixed(b);
	fa = fa - fb;//1
	fa = fa - fb;//2
	fa = fa - fb;//3
	fa = fa - fb;//4
	fa = fa - fb;//5
	fa = fa - fb;//6
	fa = fa - fb;//7
	fa = fa - fb;//8
	fa = fa - fb;//9
	fa = fa - fb;//10
	fa = fa - fb;//1
	fa = fa - fb;//2
	fa = fa - fb;//3
	fa = fa - fb;//4
	fa = fa - fb;//5
	fa = fa - fb;//6
	fa = fa - fb;//7
	fa = fa - fb;//8
	fa = fa - fb;//9
	fa = fa - fb;//10
	return (fixed_to_float(fa));
}

float	float_test_multiplication(float a, float b)
{
	a = a * b; //1
	a = a * b; //2
	a = a * b; //3
	a = a * b; //4
	a = a * b; //5
	a = a * b; //6
	a = a * b; //7
	a = a * b; //8
	a = a * b; //9
	a = a * b; //10
	a = a * b; //1
	a = a * b; //2
	a = a * b; //3
	a = a * b; //4
	a = a * b; //5
	a = a * b; //6
	a = a * b; //7
	a = a * b; //8
	a = a * b; //9
	a = a * b; //10
	return (a);
}

float	fixed_test_multiplication(float a, float b)
{
	t_fixed	fa;
	t_fixed	fb;

	fa = float_to_fixed(a);
	fb = float_to_fixed(b);
	fixed_mult(fa, fb); //1
	fixed_mult(fa, fb); //2
	fixed_mult(fa, fb); //3
	fixed_mult(fa, fb); //4
	fixed_mult(fa, fb); //5
	fixed_mult(fa, fb); //6
	fixed_mult(fa, fb); //7
	fixed_mult(fa, fb); //8
	fixed_mult(fa, fb); //9
	fixed_mult(fa, fb); //10
	fixed_mult(fa, fb); //1
	fixed_mult(fa, fb); //2
	fixed_mult(fa, fb); //3
	fixed_mult(fa, fb); //4
	fixed_mult(fa, fb); //5
	fixed_mult(fa, fb); //6
	fixed_mult(fa, fb); //7
	fixed_mult(fa, fb); //8
	fixed_mult(fa, fb); //9
	fixed_mult(fa, fb); //10
	return (fixed_to_float(fa));
}

float	float_test_devision(float a, float b)
{
	a = a / b; //1
	a = a / b; //2
	a = a / b; //3
	a = a / b; //4
	a = a / b; //5
	a = a / b; //6
	a = a / b; //7
	a = a / b; //8
	a = a / b; //9
	a = a / b; //10
	a = a / b; //1
	a = a / b; //2
	a = a / b; //3
	a = a / b; //4
	a = a / b; //5
	a = a / b; //6
	a = a / b; //7
	a = a / b; //8
	a = a / b; //9
	a = a / b; //10
	return (a);
}

float	fixed_test_devision(float a, float b)
{
	t_fixed	fa;
	t_fixed	fb;

	fa = float_to_fixed(a);
	fb = float_to_fixed(b);
	fixed_dev(fa, fb); //1
	fixed_dev(fa, fb); //2
	fixed_dev(fa, fb); //3
	fixed_dev(fa, fb); //4
	fixed_dev(fa, fb); //5
	fixed_dev(fa, fb); //6
	fixed_dev(fa, fb); //7
	fixed_dev(fa, fb); //8
	fixed_dev(fa, fb); //9
	fixed_dev(fa, fb); //10
	fixed_dev(fa, fb); //1
	fixed_dev(fa, fb); //2
	fixed_dev(fa, fb); //3
	fixed_dev(fa, fb); //4
	fixed_dev(fa, fb); //5
	fixed_dev(fa, fb); //6
	fixed_dev(fa, fb); //7
	fixed_dev(fa, fb); //8
	fixed_dev(fa, fb); //9
	fixed_dev(fa, fb); //10
	return (fixed_to_float(fa));
}

float	float_test_devision_fi(float a, float b)
{
	(void)b;
	a = a / 1; //1
	a = a / 2; //2
	a = a / 3; //3
	a = a / 4; //4
	a = a / 5; //5
	a = a / 6; //6
	a = a / 7; //7
	a = a / 8; //8
	a = a / 9; //9
	a = a / 10; //10
	a = a / 11; //1
	a = a / 12; //2
	a = a / 13; //3
	a = a / 14; //4
	a = a / 15; //5
	a = a / 16; //6
	a = a / 17; //7
	a = a / 18; //8
	a = a / 19; //9
	a = a / 20; //10
	return (a);
}

float	fixed_test_devision_fi(float a, float b)
{
	t_fixed	fa;

	(void)b;
	fa = float_to_fixed(a);
	fa = fa / 1; //1
	fa = fa / 2; //2
	fa = fa / 3; //3
	fa = fa / 4; //4
	fa = fa / 5; //5
	fa = fa / 6; //6
	fa = fa / 7; //7
	fa = fa / 8; //8
	fa = fa / 9; //9
	fa = fa / 10; //10
	fa = fa / 11; //1
	fa = fa / 12; //2
	fa = fa / 13; //3
	fa = fa / 14; //4
	fa = fa / 15; //5
	fa = fa / 16; //6
	fa = fa / 17; //7
	fa = fa / 18; //8
	fa = fa / 19; //9
	fa = fa / 20; //10
	return (fixed_to_float(fa));
}

float	float_test_additon_fi(float a, float b)
{
	(void)b;
	a = a + 1; //1
	a = a + 2; //2
	a = a + 3; //3
	a = a + 4; //4
	a = a + 5; //5
	a = a + 6; //6
	a = a + 7; //7
	a = a + 8; //8
	a = a + 9; //9
	a = a + 10; //10
	a = a + 11; //1
	a = a + 12; //2
	a = a + 13; //3
	a = a + 14; //4
	a = a + 15; //5
	a = a + 16; //6
	a = a + 17; //7
	a = a + 18; //8
	a = a + 19; //9
	a = a + 20; //10
	return (a);
}




float	fixed_test_additon_fi(float a, float b)
{
	t_fixed	fa;

	(void)b;
	fa = float_to_fixed(a);
	fa = fa + 1; //1
	fa = fa + 2; //2
	fa = fa + 3; //3
	fa = fa + 4; //4
	fa = fa + 5; //5
	fa = fa + 6; //6
	fa = fa + 7; //7
	fa = fa + 8; //8
	fa = fa + 9; //9
	fa = fa + 10; //10
	fa = fa + 11; //1
	fa = fa + 12; //2
	fa = fa + 13; //3
	fa = fa + 14; //4
	fa = fa + 15; //5
	fa = fa + 16; //6
	fa = fa + 17; //7
	fa = fa + 18; //8
	fa = fa + 19; //9
	fa = fa + 20; //10
	return (fixed_to_float(fa));
}

float	float_linear_comb_test_fn(float a, float b)
{
	float	x;
	float	y;

	x = a * 2.5f + b * 3.5f;
	y = a * 4.5f - b * 1.5f;
	return (x + y);
}

float	fixed_linear_comb_test_fn(float a, float b)
{
	t_fixed fa;
	t_fixed fb;
	t_fixed fc;
	t_fixed fd;
	t_fixed fe;

	fa = float_to_fixed(a);
	fb = float_to_fixed(b);
	fc = fixed_mult(fa, float_to_fixed(2.5f)) + fixed_mult(fb, float_to_fixed(3.5f));
	fd = fixed_mult(fa, float_to_fixed(4.5f)) - fixed_mult(fb, float_to_fixed(1.5f));
	fe = fc + fd;
	return fixed_to_float(fe);
}

int64_t	ff_test_loop(float f(float, float), char *timer_msg, int64_t time_sofar)
{
	struct timeval	start_time;
	struct timeval	end_time;
	struct timeval	diff;
	const float		goal = 10000000;
	const float		start = -10000000;
	float			a = start;
	float			b = start;
	float			return_val;

	usleep(100000);
	int64_t	arr[10000];
	float	arr2[10000];
	for (int i = 0; i < 10000; i++)
	{
		arr[i] = i;
		arr2[i] = i;
	}
	start_time = timer(NULL);
	while (a < goal)
	{
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
