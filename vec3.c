
#include <cub3D.h>
#include <MLX42.h>


t_vec3 v3_zero(void)
{
	t_vec3	v;
	int		n = 3;

	while (n--)
	{
		v.p[n] = 0;
	}
	return (v);
}

t_vec3 v3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	v;
	int		n = 3;

	while (n--)
	{
		v.p[n] = a.p[n] + b.p[n];
	}
	return (v);

}
t_vec3 v3_reverse(t_vec3 a)
{
	t_vec3	v;
	int		n = 3;

	while (n--)
	{
		v.p[n] = -a.p[n];
	}
	return (v);

}
t_vec3 v3_multiply(t_vec3 a, t_vec3 b)
{
	t_vec3	v;
	int		n = 3;

	while (n--)
	{
		v.p[n] = a.p[n] * b.p[n];
	}
	return (v);
}

t_vec3 v3_scale(t_vec3 a, float k) 
{
	t_vec3	v;
	int		n = 3;

	while (n--)
	{
		v.p[n] = a.p[n] * k;
	}
	return v;
}

t_vec3 v3_random(void)
{
	t_vec3	v;
	int		n = 3;
	

	while (n--)
	{
		v.p[n] = (double)rand() / (double) RAND_MAX;
		if (n == 0 || n == 1)
		{
			v.p[n] *= 4.0;
			v.p[n] -= 1.0;
		}
		else
		{
			// v.p[n] += 1;
			// v.p[n] *= 6;
			v.p[n] = 1;
		}
	}
	print_vec3(v, "a");
	return (v);
}

void zero_vec3(t_vec3 *v)
{
	int n = 3;

	while (n--)
	{
		v->p[n] = 0;
	}
}

void add_vec3(t_vec3 *v, t_vec3 *a)
{
	int n = 3;

	while (n--)
	{
		v->p[n] += a->p[n];
	}
}
void reverse_vec3(t_vec3 *v)
{
	int n = 3;

	while (n--)
	{
		v->p[n] *= -1;
	}
}
void multiply_vec3(t_vec3 *v, t_vec3 *a)
{
	int n = 3;

	while (n--)
	{
		v->p[n] += a->p[n];
	}
}

void scale_vec3(t_vec3 *v, float k) 
{
	t_vec3	a;
	int		n = 3;

	while (n--)
	{
		a.p[n] = v->p[n] * k;
	}
	*v = a;
}

float length_vec3(t_vec3 *v)
{
	float	sum = 0;
	int		n = 3;

	while (n--)
	{
		sum += powf(v->p[n], 2);
	}
	return sqrtf(sum);
}

void	print_vec3(t_vec3 v, char *msg)
{
	if (!msg)
		printf("x: %.2f, y: %.2f z: %.2f\n", v.p[0], v.p[1], v.p[2]);
	else
		printf("%s x: %.2f, y: %.2f z: %.2f\n", msg, v.p[0], v.p[1], v.p[2]);
}
