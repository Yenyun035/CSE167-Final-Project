/**************************************************
a class for ray information
*****************************************************/
#ifndef __RAY_H__
#define __RAY_H__
#include <glm/glm.hpp>

struct Ray {
    glm::vec3 p0;
    glm::vec3 d;
};

#endif 