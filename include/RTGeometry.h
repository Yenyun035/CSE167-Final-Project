#include <vector>
#include "Triangle.h"

#ifndef __RTGEOMETRY_H__
#define __RTGEOMETRY_H__

class RTGeometry {
public:
  uint64_t count = 0; // number of elements to draw
  std::vector<Triangle> elements; // list of triangles
  
  virtual void init(){};
  virtual void init(const char* s){};
};

#endif