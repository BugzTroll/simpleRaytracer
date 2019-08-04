#pragma once
#include "hitable.h"

class sphere: public hitable
{
public:
    sphere();
    sphere(vec3 cent, float r) : center(cent), radius(r) {};
    ~sphere();

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
    vec3 center;
    float radius;
};

