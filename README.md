# CSE167-Final-Project

Original code from cse167 @ ucsd HW3 (2022 Fall)

## Code Changelog
- [Nov 15] Triangle class, Ray class, Intersection class, Intersect method

## TODO:
- Geometry class modification
- Cube class modification
- Obj class modification
- Find Triangle Soup
- Image
- RayThruPixel
- FindColor
- RayTrace

## Question:
- The ray parameter d is the ray direction. Is "direction of source to the incoming ray" in intersection class the same as ray direction?
> It would be a modification of the Geometry class, and their inherited class Cube and Obj. If you look at those classes, Geometry contains buffers containing the vertex positions and connectivity for OpenGL pipeline to draw elements.  Cube and Obj have the implementation of filling up those buffers. In ray tracing, we can modify Geometry class to a simpler one.  It contains **a list of triangle.  (One needs to write a "Triangle" class which has 3 vertex positions, 3 normals, and perhaps a pointer to material.)** And modify the Cube and Obj so that instead of filling up vertex buffers, just fill in the list of triangles. After that, when we build the scene, every time we add a new cube, teapot or bunny in the geometry palette, we have some list of triangles constructed. During the DFS, as we **dive into one of the model, we access those triangles in the corresponding geometry, copy and paste the triangle into the triangle soup.**  Note that as we dump the triangles into triangle soup, we need to apply the transformation so that **all the coordinates in the soup are relative to the world or relative to camera** for later convenience.
