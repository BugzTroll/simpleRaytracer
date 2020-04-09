#ifndef CAMERAH
#define CAMERAH

#include "ray.h"
#include <corecrt_math_defines.h>
#include <math.h> 
#include "utilities.h"

class camera
{
public:
	camera(vec3 lookFrom, vec3 lookAt, vec3 vUp, float vfov, float aspect, float aperture, float focusDist)
	{
		lensRadius = aperture / 2;

		float theta = vfov * M_PI / 180.0;
		float div = theta / float(2.0);
		float halfHeight = tan(div);
		float halfWidth = aspect * halfHeight;

		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(vUp, w));
		v = unitVector(cross(w, u));

		lowerLeftCorner = origin - (halfWidth * focusDist * u) - (halfHeight * focusDist * v) - focusDist *w;
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	ray getRay(float s, float t)
	{
		vec3 rd = lensRadius * utilities::randomInUnitDisk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
	}

	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
	vec3 u;
	vec3 v;
	vec3 w;
	float lensRadius;
};

#endif