#ifndef LAMBERTIANH
#define LAMBERTIANH

#include "material.h"
#include "utilities.h"
#include "hitable.h"

class lambertian : public material
{
public:
	lambertian(const vec3& a) : albedo(a) {};
	virtual bool scatter(const ray& rIn, const hitRecord rec, vec3& attenuation, ray& scattered) const
	{
		vec3 target = rec.p + rec.normal + utilities::randomInUnitSphere();
		scattered = ray(rec.p, target - rec.p);
		attenuation = albedo;
		return true;
	}
	
	vec3 albedo;
};
#endif