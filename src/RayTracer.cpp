#include "RayTracer.h"
#include <glm/glm.hpp>
#include <limits>

namespace RayTracer {
    void Raytrace(Camera cam, RTScene scene, Image &image){

    };

    Ray RayThruPixel(Camera cam, int i, int j, int width, int height) {
        return NULL;
    };

    Intersection Intersect(Ray ray, Triangle tri) {
        glm::vec3 p1 = tri.pos[0];
        glm::vec3 p2 = tri.pos[1];
        glm::vec3 p3 = tri.pos[2];
        glm::vec3 d = ray.d;
        glm::vec3 p0 = ray.p0;
        
        glm::mat4 m;
        m[0] = p1;
        m[1] = p2;
        m[2] = p3;
        m[3] = glm::vec4(-d, 0.0f);
        
        glm::vec4 lamdaT = glm::inverse(m) * glm::vec4(p0, 1.0f);

        Intersection inter;

        if(lamdaT.x >= 0 && lamdaT.y >= 0 && lambaT.z >= 0 && lamdaT.w >= 0) {
            // position q i.e. intersection point
            inter.intsec_pos = glm::vec3(lamdaT.x * p1 + lamdaT.y * p2 + lamdaT.z * p3);
            
            // vector normal to q
            inter.surface_normal = glm::normalize(glm::vec3(lamdaT.x * tri.normal[0] 
                                    + lamdaT.y * ptri.normal[1] + lamdaT.z * tri.normal[2]));
            // incoming ray direction
            inter.inr_d = d;

            // pointer to the triangle
            inter.tri = &tri;
            
            // t, i.e distance to ray
            inter.dist = lamdaT.w;
            
        } else {
            inter.dist = std::numeric_limits<float>::max();
        }

        return inter;
    } //page 30, 33

    Intersection Intersect(Ray ray, RTScene scene) {
        float mindist = std::numeric_limits<float>::max();
        Intersection hit = new Intersection();
        
        for(Triangle tri : scene.triangle_soup){
            // Find closest intersection; test all triangles
            Intersection hit_temp = Intersect(ray, tri);
            // closer than previous hit
            if (hit_temp.dist < mindist) {
                mindist = hit_temp.dist;
                hit = hit_temp;
            }
        }
        return hit;
    } //page 11, 28, 31

    glm::vec3 FindColor(Intersection hit, int recursion_depth) {
        return NULL;
    } //page 15
};