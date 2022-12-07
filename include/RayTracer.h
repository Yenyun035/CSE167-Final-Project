#include "Camera.h"
#include "RTScene.h"
#include "Image.h"
#include "Intersection.h"
#include "Ray.h"
#include "Triangle.h"
#include <glm/glm.hpp>

#ifndef __RAYTRACER_H__
#define __RAYTRACER_H_

namespace RayTracer {
    void Raytrace(Camera cam, RTScene scene, Image &image); //page 9
    Ray RayThruPixel(Camera cam, float i, float j, int width, int height);//page 10, 18
    Intersection Intersect(Ray ray, Triangle * triangle); //page 30, 33
    Intersection Intersect(Ray ray, RTScene * scene); //page 11, 28, 31
    glm::vec3 FindColor(Intersection hit, int recursion_depth, RTScene * scene); //page 15
};

#endif