#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera
{
public:
	camera()
	{
		lowerLeftCorner = vec3(-2.0, -1.0, -1.0);
		horizontal = vec3(4.0, 0.0, 0.0);
		vertical = vec3(0.0, 2.0, 0.0);
		origin = vec3(0.0, 0.0, 0.0);
	}

	ray getRay(float u, float v)
	{
		return ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
	}

	vec3 lowerLeftCorner;
	vec3 horizontal;
	vec3 vertical;
	vec3 origin;
};

#endif