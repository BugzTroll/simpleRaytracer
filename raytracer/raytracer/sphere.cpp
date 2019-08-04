#include "sphere.h"

sphere::sphere()
{
}

sphere::~sphere()
{
}

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {

    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0){
        float t = (-b - sqrt(discriminant)) / (2.0 * a);
        if(t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.normal = unit_vector(rec.p - center) / radius;
            return true;
        }
        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if(t < t_max && t > t_min){
            rec.t = t;
            rec.p = r.point_at_parameter(t);
            rec.normal = unit_vector(rec.p - center) / radius;
            return true;
        }
    } 
       return false;
}
