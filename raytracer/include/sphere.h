#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {};
	sphere(vec3 center, float radius, material* mat) :
		center(center),
		radius(radius),
		material(mat)
	{};
	virtual bool hit(const ray& r, float tmin, float tmax, hitRecord& rec) const;
	vec3 center;
	float radius;
	material *material;
};

bool sphere::hit(const ray& r, float tmin, float tmax, hitRecord& rec) const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminent = b * b - a * c;
	if (discriminent > 0)
	{
		float temp = (-b - sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = material;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < tmax && temp > tmin)
		{
			rec.t = temp;
			rec.p = r.pointAtParameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.matPtr = material;
			return true;
		}
	}
	return false;
}
#endif
