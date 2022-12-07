/**************************************************
a class for intersection information
*****************************************************/
#ifndef __INTERSECTION_H__
#define __INTERSECTION_H__
#include <glm/glm.hpp>
#define INF_DIST 2.0e+10

class Intersection {
public:
    glm::vec3 intsec_pos; // i j k
    glm::vec3 surface_normal;
    glm::vec3 inr_d;
    Triangle * tri;
    float dist = INF_DIST;

    virtual void init(){};
};

#endif 