#ifndef DIELECTRICH
#define DIELECTRICH

#include "material.h"
#include "utilities.h"
#include "hitable.h"

class dielectric : public material
{
public:
	dielectric(float ri) : refIdx(ri) {};
	virtual bool scatter(const ray& rIn, const hitRecord rec, vec3& attenuation, ray& scattered) const
	{
		vec3 outwardNormal;
		vec3 reflected = utilities::reflect(rIn.direction(), rec.normal);
		float niOverNt = 0.0;
		attenuation = vec3(1.0, 1.0, 1.0);
		vec3 refracted;
		float reflectProb;
		float cosine = 0.0;
		if (dot(rIn.direction(), rec.normal) > 0)
		{
			outwardNormal = -rec.normal;
			niOverNt = refIdx;
			cosine = refIdx * dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}
		else
		{
			outwardNormal = rec.normal;
			niOverNt = 1.0 / refIdx;
			cosine = -dot(rIn.direction(), rec.normal) / rIn.direction().length();
		}
		if (utilities::refract(rIn.direction(), outwardNormal, niOverNt, refracted))
		{
			reflectProb = utilities::shlick(cosine, refIdx);
		}
		else
		{
			reflectProb = 1.0;
			scattered = ray(rec.p, reflected);
		}
		if ((rand() / double(RAND_MAX)) < reflectProb)
		{
			scattered = ray(rec.p, reflected);
		}
		else
		{
			scattered = ray(rec.p, refracted);
		}
		return true;
	}

	float refIdx;
};
#endif