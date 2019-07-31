#include <iostream>
#include <fstream>
#include "vec3.h"
#include "vec3.cpp"
#include "ray.h"
#include <math.h>

using namespace std;

float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if(discriminant < 0){
        return -1.0;
    }else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

vec3 color(const ray& r){
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
    if(t > 0.0){
        vec3 pointOnSphere = r.point_at_parameter(t);
        vec3 normal = unit_vector(pointOnSphere - vec3(0, 0, -1));
        cout << r.point_at_parameter(t).z();
        //cout << normal.z() << endl;
        return vec3(normal.x() + 1, normal.y() + 1, normal.z() + 1) * 0.5;
    }
    vec3 unit_direction = unit_vector(r.direction());
    float a = 0.5 * (unit_direction.y() + 1.0);
    return (vec3(1.0, 1.0, 1.0) * (1.0 - a)) + (vec3(0.5, 0.7, 1.0) * a);
}
int main()
{
    cout << "Writing to file";
  
    ofstream myfile;
    myfile.open("outputImages/outputImage.ppm");

    int nx = 200;
    int ny = 100;
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {
        for(int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin, unit_vector(lower_left_corner + horizontal * u + vertical * v));
            vec3 col = color(r);
            int ir = int(255.99*col.r());
            int ig = int(255.99*col.g());
            int ib = int(255.99*col.b());
            myfile << ir << " " << ig << " " << ib << "\n";
        }
    }   
    myfile.close();

    cin >> nx;
    return 0;
}