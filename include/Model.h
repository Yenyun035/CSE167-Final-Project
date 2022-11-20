/**************************************************
A model is a geometry with material.
*****************************************************/
// #include "Geometry.h"
#include "RTGeometry.h"
#include "Material.h"

#ifndef __MODEL_H__
#define __MODEL_H__

struct Model {
    RTGeometry* geometry;
    Material* material;
};

#endif 
