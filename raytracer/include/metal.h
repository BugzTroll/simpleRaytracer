#ifndef METALH
#define METALH

#include "material.h"
#include "utilities.h"

class metal : public material
{
public:
	metal(const vec3& a,float f) : albedo(a) 
	{
		fuzz = f < 1 ? f : 1;
	};
	virtual bool scatter(const ray& rIn, const hitRecord rec, vec3& attenuation, ray& scattered) const const
	{
		vec3 reflected = utilities::reflect(unitVector(rIn.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * utilities::randomInUnitSphere());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	vec3 albedo;
	float fuzz;
};

#endif
