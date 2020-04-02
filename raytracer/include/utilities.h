#ifndef UTILITIESH
#define UTILITIESH

#include "vec3.h"

namespace utilities 
{
	vec3 randomInUnitSphere()
	{
		vec3 p;
		do
		{
			p = 2.0 * vec3(rand() / double(RAND_MAX), rand() / double(RAND_MAX), rand() / double(RAND_MAX)) - vec3(1, 1, 1);
		} while (p.square_length() >= 1);
		return p;
	};

	vec3 reflect(const vec3& v, const vec3& n)
	{
		return v - 2 * dot(v, n) * n;
	}

	bool refract(const vec3& v, const vec3& n, float niOverNt, vec3& refracted)
	{
		vec3 uv = unitVector(v);
		float dt = dot(uv, n);
		float discriminant = 1.0 - (niOverNt * niOverNt) * (1 - dt * dt);
		if (discriminant > 0)
		{
			refracted = niOverNt * (uv - n * dt) - n * sqrt(discriminant);
			return true;
		}
		else
		{
			return false;
		}
	}

	float shlick(float cosine, float refIdx)
	{
		float r0 = (1 - refIdx) / (1 + refIdx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
}

#endif