
#include <cub3D.h>
#include <MLX42.h>


t_vec3 v3_zero(void)
{
	t_vec3	v;

	v.x = 0;
	v.y = 0;
	v.z = 0;
	return (v);
}

t_vec3 v3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	return (v);

}
t_vec3 v3_reverse(t_vec3 a)
{
	t_vec3	v;

	v.x = -a.x;
	v.y = -a.y;
	v.z = -a.z;
	return (v);
}
t_vec3 v3_multiply(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	return (v);
}

t_vec3 v3_scale(t_vec3 a, float scalar)
{
	t_vec3	v;

	v.x = a.x * scalar;
	v.y = a.y * scalar;
	v.z = a.z * scalar;
	return v;
}

t_vec3 v3_random(void)
{
	t_vec3	v;

	v.x = (double)rand() / (double) RAND_MAX;
	v.x *= 4.0;
	v.x -= 1.0;
	v.y = (double)rand() / (double) RAND_MAX;
	v.y *= 4.0;
	v.y -= 1.0;
	v.z = (double)rand() / (double) RAND_MAX;
	// v.z += 1;
	// v.z *= 6;
	v.z = 1;
	//print_vec3(v, "a");
	return (v);
}

void zero_vec3(t_vec3 *v)
{
	v->x = 0;
	v->y = 0;
	v->z = 0;
}

void add_vec3(t_vec3 *v, t_vec3 *a)
{
	v->x += a->x;
	v->y += a->y;
	v->z += a->z;
}

void reverse_vec3(t_vec3 *v)
{
	v->x *= -1;
	v->y *= -1;
	v->z *= -1;
}

void multiply_vec3(t_vec3 *v, t_vec3 *a)
{
	v->x *= a->x;
	v->y *= a->y;
	v->z *= a->z;
}

void scale_vec3(t_vec3 *v, float scalar)
{
	v->x *= scalar;
	v->y *= scalar;
	v->z *= scalar;
}

float length_vec3(t_vec3 *v)
{
	float	sum;

	sum = powf(v->x, 2);
	sum += powf(v->y, 2);
	sum += powf(v->z, 2);
	return sqrtf(sum);
}

void	print_vec3(t_vec3 v, char *msg)
{
	if (!msg)
		printf("x: %.2f, y: %.2f z: %.2f\n", v.x, v.y, v.z);
	else
		printf("%s x: %.2f, y: %.2f z: %.2f\n", msg, v.x, v.y, v.z);
}
