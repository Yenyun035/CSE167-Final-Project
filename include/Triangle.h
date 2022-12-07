#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include "Material.h"
#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

struct Triangle {
    std::vector<glm::vec3> pos; // 3 positions //vec before
    std::vector<glm::vec3> normal; // 3 normals
    Material* material;
};

#endif