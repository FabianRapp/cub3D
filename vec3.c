
#include <cub3D.h>
#include <MLX42.h>


t_vec3	v3_zero(void)
{
	t_vec3	v;

	v.x = 0;
	v.y = 0;
	v.z = 0;
	v.w = 0;
	return (v);
}

void	unit_vec3(t_vec3 *v)
{
	float	len;
	float	sum;

	sum = v->x * v->x + v->y * v->y + v->z * v->z;
	if (sum > 0.0001 || sum < -0.0001)
	{
		len = sqrtf(sum);
		v->x /= len;
		v->y /= len;
		v->z /= len;
	}
	else if (v->w == 3)
		fprintf(stderr, "unable to make uinit vec: to close to zero: %f\n", sum);
}

// rotation_matrix_x
//         [1, 0, 0],
//         [0, np.cos(theta), -np.sin(theta)],
//         [0, np.sin(theta), np.cos(theta)]


// rotation_matrix_y
//         [np.cos(phi), 0, np.sin(phi)],
//         [0, 1, 0],
//         [-np.sin(phi), 0, np.cos(phi)]


// rotation_matrix_z
//         [np.cos(psi), -np.sin(psi), 0],
//         [np.sin(psi), np.cos(psi), 0],
//         [0, 0, 1]



//{
// {cos α cos β cos γ − sin α sin γ, − cos α cos β sin γ − sin α cos γ, cos α sin β },
// {sin α cos β cos γ + cos α sin γ, − sin α cos β sin γ + cos α cos γ, sin α sin β },
// {− sin β cos γ, sin β sin γ, cos β }
//}


// angle in rad
void	rotate_vec3(t_vec3 *to_rotate, float x_rot, float first_z_rot, float second_z_rot)
{
	float cx = cosf(x_rot);
	float sx = sinf(x_rot);
	float cz1 = cosf(first_z_rot);
	float sz1 = sinf(first_z_rot);
	float cz2 = cosf(second_z_rot);
	float sz2 = sinf(second_z_rot);

	t_vec3	re;
	float	rotation_matrix[3][3] = {
		{cz1 * cx * cz2 - sz1 * sz2, -cz1 * cx * sz2 - sz1 * cz2, cz1 * sx},
		{sz1 * cx * cz2 + cz1 * sz2, -sz1 * cx * sz2 + cz1 * cz2, sz1 * sx},
		{-sx * cz2, sx * sz2, cx}
	};
	re.x = to_rotate->x *  rotation_matrix[0][0] + to_rotate->y * rotation_matrix[1][0] + to_rotate->z * rotation_matrix[2][0];
	re.y = to_rotate->x *  rotation_matrix[0][1] + to_rotate->y * rotation_matrix[1][1] + to_rotate->z * rotation_matrix[2][1];
	re.z = to_rotate->x *  rotation_matrix[0][2] + to_rotate->y * rotation_matrix[1][2] + to_rotate->z * rotation_matrix[2][2];
	to_rotate->x = re.x;
	to_rotate->y = re.y;
	to_rotate->z = re.z;
	// // Apply Z-axis rotation
	// float xy = cz * to_rotate->x - sz * to_rotate->y;
	// float yy = sz * to_rotate->x + cz * to_rotate->y;
	// float zy = to_rotate->z;

	// // Apply Y-axis rotation
	// float xz = cy * xy + sy * zy;
	// float yz = yy;
	// float zz = -sy * xy + cy * zy;

	// // Apply X-axis rotation
	// to_rotate->x = xz;
	// to_rotate->y = cx * yz - sx * zz;
	// to_rotate->z = sx * yz + cx * zz;
}

t_vec3	cross_product(t_vec3 a, t_vec3 b)
{
	t_vec3	result;

	result.x = a.y * b.z - a.z * b.y;
	result.y = a.z * b.x - a.x * b.z;
	result.z = a.x * b.y - a.y * b.x;
	result.w = 1;
	return (result);
}

float	dot_prod_unit(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

t_vec3	v3_add(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	v.w = a.w;
	return (v);
}

t_vec3	v3_sub(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	v.w = 1;
	return (v);
}

t_vec3 v3_reverse(t_vec3 a)
{
	t_vec3	v;

	v.x = -a.x;
	v.y = -a.y;
	v.z = -a.z;
	v.w = 1;
	return (v);
}
t_vec3 v3_multiply(t_vec3 a, t_vec3 b)
{
	t_vec3	v;

	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	v.w = 1;
	return (v);
}

t_vec3 v3_scale(t_vec3 a, float scalar)
{
	t_vec3	v;

	v.x = a.x * scalar;
	v.y = a.y * scalar;
	v.z = a.z * scalar;
	v.w = 1;
	return v;
}



t_vec3 v3_random(void)
{
	t_vec3	v;
	static int i = 1;
	static int a;
	
	v.x = generate_random_float();
	//((float)rand()) / RAND_MAX;
	fprintf(stderr, "%f\n", v.x);
	//v.x -= 0.5;
	//v.x *= 2;
	// v.x *= 4.0;
	// v.x -= 1.0;
	v.y = generate_random_float();
	//((double)rand()) / (double) RAND_MAX;
	// v.y *= 4.0;
	// v.y -= 1.0;
	//v.z = generate_random_float();
	//((double)rand()) / (double) RAND_MAX;
	// v.z += 1;
	// v.z *= 6;
	v.z = 1;
	//print_vec3(v, "a");
	//v.w = 1;
	return (v);
}

void zero_vec3(t_vec3 *v)
{
	v->x = 0;
	v->y = 0;
	v->z = 0;
//	v->w = 1;
}

void add_vec3(t_vec3 *v, t_vec3 *a)
{
	v->x += a->x;
	v->y += a->y;
	v->z += a->z;
	//v->w += a->w;
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

void div_vec3(t_vec3 *v, float a)
{
	if (zero_f(a))
	{
		fprintf(stderr, "zero div!\n");
		exit(1);
	}
	v->x /= a;
	v->y /= a;
	v->z /= a;
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
	{
		fprintf(stderr, "x: %.2f, y: %.2f z: %.2f w: %.2f u: %.2f v: %.2f", v.x, v.y, v.z, v.w, v.u, v.v);
		if (v.mtl)
		{
			fprintf(stderr, "\n");
			//fprintf(stderr, " mtl: %s\n", v.mtl->name);
		}
		else
		{
			fprintf(stderr, " mtl: none\n");
		}
	}
	else
	{
		fprintf(stderr, "%s x: %.2f, y: %.2f z: %.2f w: %.2f u: %.2f v: %.2f", msg, v.x, v.y, v.z, v.w, v.u, v.v);
		if (v.mtl)
		{
			fprintf(stderr, "\n");
			//fprintf(stderr, " mtl: %s\n", v.mtl->name);
		}
		else
		{
			fprintf(stderr, " mtl: none\n");
		}
	}
	// if (!msg)
	// {
	// 	fprintf(stderr, "x: %.2f, y: %.2f z: %.2f w: %.2f u: %.2f v: %.2f", v.x, v.y, v.z, v.w, v.u, v.v);
	// 	if (v.mtl)
	// 	{
	// 		fprintf(stderr, " mtl: %s\n", v.mtl->name);
	// 	}
	// 	else
	// 	{
	// 		fprintf(stderr, " mtl: none\n");
	// 	}
	// }
	// else
	// {
	// 	fprintf(stderr, "%s x: %.2f, y: %.2f z: %.2f w: %.2f u: %.2f v: %.2f", msg, v.x, v.y, v.z, v.w, v.u, v.v);
	// 	if (v.mtl)
	// 	{
	// 		fprintf(stderr, " mtl: %s\n", v.mtl->name);
	// 	}
	// 	else
	// 	{
	// 		fprintf(stderr, " mtl: none\n");
	// 	}
	// }
}

void	init_vec3(t_vec3 *v, float x, float y, float z)
{
	v->x = x;
	v->y = y;
	v->z = y;
	v->w = 1;
}

t_vec3	vec3_init(float x, float y, float z)
{
	t_vec3	v;

	v.x = x;
	v.y = y;
	v.z = y;
	v.w = 1;
	return (v);
}

t_vec3	get_direction(float pitch, float yaw, float roll)
{
	t_vec3	direct;

	direct.w = 1;
	direct.x = cosf(pitch) * sinf(yaw);
	direct.y = sin(pitch);
	direct.z = cos(pitch) * cos(yaw);
	return (direct);
}
