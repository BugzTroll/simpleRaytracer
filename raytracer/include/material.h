#ifndef MATERIALH
#define MATERIALH

#include "ray.h"

class hitRecord;

class material
{
public:
	virtual bool scatter(const ray& r_in, const hitRecord rec, vec3& attenuation, ray& scattered) const = 0;
};

#endif