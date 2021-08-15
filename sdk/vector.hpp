#pragma once

#include <cmath>

struct s_vector2 {
	float x, y;
};

struct s_vector3
{
	float x, y, z;

	s_vector3() : x(0.0f), y(0.0f), z(0.0f) {};
	s_vector3(float x, float y, float z) : x(x), y(y), z(z) {};
	s_vector3(c_navigation right) : x(right.position.x), y(right.position.y), z(right.position.z) {};

	s_vector3 operator-(s_vector3 right)
	{
		s_vector3 result = *this;

		result.x -= right.x;
		result.y -= right.y;
		result.z -= right.z;

		return result;
	}

	s_vector3 operator*(s_vector3 right)
	{
		s_vector3 result;

		result.x = this->x * right.x;
		result.y = this->y * right.y;
		result.z = this->z * right.z;

		return result;
	}

	float distance(s_vector3 right)
	{
		s_vector3 d = right - *this;
		return std::sqrtf(d.x * d.x + d.y * d.y + d.z * d.z);
	}
};