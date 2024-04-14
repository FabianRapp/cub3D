
#include <cub3D.h>
#include <MLX42.h>


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

t_vec3 scale_vec3(t_vec3 *v, float k) 
{
	t_vec3	a;
	int		n = 3;

	while (n--)
	{
		a.p[n] = v->p[n] * k;
	}
	return a;
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

void	print_vec3(t_vec3 *v, char *msg)
{
	if (!msg)
		printf("x: %f, y: %f z: %f\n", v->p[0], v->p[1], v->p[2]);
	else
		printf("%s x: %f, y: %f z: %f\n", msg, v->p[0], v->p[1], v->p[2]);
}
