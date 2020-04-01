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
}

#endif