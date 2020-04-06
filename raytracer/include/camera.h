#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include <corecrt_math_defines.h>
#include <math.h> 

class camera
{
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect)
	{
		vec3 u, v, w;

		float theta = vfov * M_PI / 180.0;
		float div = theta / float(2.0);
		float halfHeight = tan(div);
		float halfWidth = aspect * halfHeight;

		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vUp, w));
		v = unitVector(cross(w, u));

		lowerLeftCorner = origin - (halfWidth * u) - (halfHeight * v) - w;
		horizontal = 2 * halfWidth * u;
		vertical = 2 * halfHeight * v;
	}

	ray getRay(float s, float t)
	{
		return ray(origin, lowerLeftCorner + s * horizontal + t * vertical - origin);
	}

	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
};

#endif