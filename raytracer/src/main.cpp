#include <iostream>
#include <fstream>
#include <limits>
#include <stdlib.h>
#include "vec3.h"
#include "vec3.cpp"
#include "ray.h"
#include <math.h>
#include "hitableList.h"
#include "sphere.h"
#include "camera.h"

using namespace std;

vec3 color(const ray& r, hitable* world)
{
	hitRecord rec;
	if (world->hit(r, 0.0, numeric_limits<float>::max(), rec))
	{
		return (0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1));
	}
	else
	{
		vec3 unitDirection = unitVector(r.direction());
		float t = 0.5 * (unitDirection.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
int main()
{
	cout << "Writing to file" << endl;

	// open image file
	ofstream myfile("../outputImages/outputImage.ppm");

	int nx = 200;
	int ny = 100;
	int ns = 100;

	camera cam;

	hitable* list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable* world = new hitableList(list, 2);
	myfile << "P3\n" << nx << " " << ny << "\n255\n";

	// Send rays for every pixels
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {

			// Average the color to get antialiased image
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++)
			{
				float u = (float(i) + (rand() / double(RAND_MAX))) / float(nx);
				float v = (float(j) + (rand() / double(RAND_MAX))) / float(ny);

				ray r = cam.getRay(u, v);
				vec3 p = r.pointAtParameter(2.0);
				col += color(r, world);
			}

			col /= float(ns);

			int ir = int(255.99 * col.r());
			int ig = int(255.99 * col.g());
			int ib = int(255.99 * col.b());
			myfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	myfile.close();

	cout << "done" << endl;

	cin >> nx;
	return 0;
}