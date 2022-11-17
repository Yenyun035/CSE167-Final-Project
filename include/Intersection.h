/**************************************************
a class for intersection information
*****************************************************/
#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__
#include <glm/glm.hpp>

class Intersection {
public:
    glm::vec3 intsec_pos;
    glm::vec3 surface_normal;
    glm::vec3 inr_d;
    Triangle * tri;
    float dist;

    virtual void init(){};
    Intersection Intersect(Ray ray, Scene scene);

private:
    Intersection Intersect(Ray ray, Triangle tri);
};

#endif 