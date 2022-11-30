#include "RayTracer.h"
#include <glm/glm.hpp>
#include <limits>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#define GLM_SWIZZLE
#define INF_DIST 2.0e+10

namespace RayTracer {
    
    void Raytrace(Camera cam, RTScene scene, Image &image){
        int w = image.width;
        int h = image.height;
        int sample_num = 3;
        int rec = 1;
        float inv_sam = 1.0f / sample_num;

        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                glm::vec3 sum_color = glm::vec3(0.0f, 0.0f, 0.0f);
                
                for (int k = 0; k < sample_num; k++) {
                   float rand_i = i + float(rand()) / RAND_MAX; // [i,i+1]
                   float rand_j = j + float(rand()) / RAND_MAX; // [j,j+1]
                   Ray ray = RayThruPixel(cam, rand_i, rand_j, w, h);
                   Intersection hit = Intersect(ray, &scene);
                   sum_color += FindColor(hit, rec, &scene);
                }
            
                image.pixels[j * w + i] = inv_sam * sum_color;
            }
        }

        std::cout << "Ray Tracing completed" << std::endl;
    };

    Ray RayThruPixel(Camera cam, float i, float j, int width, int height) {
        glm::vec3 w = glm::normalize(cam.eye - cam.target);
        glm::vec3 u = glm::normalize(glm::cross(cam.up, w));
        glm::vec3 v = glm::normalize(glm::cross(w, u));

        Ray ray;
        ray.p0 = cam.eye;

        float alpha = 2 * ((i + 0.5f) / width) - 1; // does +0.5f means center of the pixel?
        float beta = 1 - (2 * (j + 0.5f) / height);
        // TODO: is aspect from camera property or from parameter of the function?
        float tan_fovy = glm::tan(cam.fovy / 2);
        
        ray.d = glm::normalize(alpha * cam.aspect * tan_fovy * u + beta * tan_fovy * v - w);

        return ray;
    };

    Intersection Intersect(Ray ray, Triangle * tri) {
        glm::vec3 p1 = tri->pos[0], p2 = tri->pos[1], p3 = tri->pos[2];
        glm::vec3 d = ray.d, p0 = ray.p0;
        
        glm::mat4 m;
        m[0] = glm::vec4(p1, 1.0f);
        m[1] = glm::vec4(p2, 1.0f);
        m[2] = glm::vec4(p3, 1.0f);
        m[3] = glm::vec4(-d, 0.0f);
        
        glm::vec4 lamdaT = glm::inverse(m) * glm::vec4(p0, 1.0f);

        Intersection inter;

        if(lamdaT.x >= 0 && lamdaT.y >= 0 && lamdaT.z >= 0 && lamdaT.w >= 0) {
            // position q i.e. intersection point
            inter.intsec_pos = glm::vec3(lamdaT.x * p1 + lamdaT.y * p2 + lamdaT.z * p3);
            
            // vector normal to q
            inter.surface_normal = glm::normalize(glm::vec3(lamdaT.x * tri->normal[0] 
                                    + lamdaT.y * tri->normal[1] + lamdaT.z * tri->normal[2]));
            // incoming ray direction
            inter.inr_d = -d;

            // pointer to the triangle
            inter.tri = tri;
            
            // t, i.e distance to ray
            inter.dist = lamdaT.w;

            std::cout << "Intersection exist distance: " << inter.dist << std::endl;
        }

        return inter;
    }; //page 30, 33

    Intersection Intersect(Ray ray, RTScene * scene) {
        float mindist = INF_DIST;
        Intersection hit;
        
        for(Triangle tri : scene->triangle_soup){
            // Find closest intersection; test all triangles
            Intersection hit_temp = Intersect(ray, &tri);
            
            // closer than previous hit
            if (hit_temp.dist < mindist) {
                mindist = hit_temp.dist;
                hit = hit_temp;
            }
        }

        return hit;
    }; //page 11, 28, 31

    glm::vec3 FindColor(Intersection hit, int recursion_depth, RTScene * scene) {
        /*
        
        if (recursion_depth == 0 && hit.dist < INF_DIST) {        
            glm::vec4 light_sum = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
            
            for(std::pair<std::string,Light*> entry: scene->light) {
                Light* light = entry.second;

                // find visibility
                int vis = 1;
                Ray ray;
                ray.p0 = hit.intsec_pos;
                ray.d = light->position - hit.intersec_pos;
                Intersection light_hit = Intersect(ray, scene);
                if (light_hit.dist == INF_DIST) { vis = 0; }

                // transform light position to camera coord
                glm::vec4 cLightPos = scene->camera->view * light->position;

                // l vector
                glm::vec3 l = normalize(glm::vec3(cLightPos) - cLightPos.w * hit.intsec_pos);

                // diffuse * max(n.l, 0)
                float diffsca = std::max(glm::dot(hit.surface_normal,l), 0.0f);
                glm::vec4 diffuse = diffsca *  hit.tri->material->diffuse;

                // diffuse * max(n.l, 0) * vis
                light_sum += vis * glm::vec4(light->color.x * diffuse.x, light->color.y * diffuse.y, light->color.z * diffuse.z, light->color.w * diffuse.w);
            }
            
            return glm::vec3(light_sum);
        }
        else if(hit.dist < INF_DIST) {
            float randChoose = double(rand()) / RAND_MAX;
            if (randChoose <= 0.5) { //diffuse
                // calculate d - TODO
                float randAngle0 = double(rand()) / RAND_MAX * M_PI;
                float randAngle1 = double(rand()) / RAND_MAX * M_PI;
                float di = glm::sin(randAngle0) * glm::sin(randAngle1);
                float dj = glm::sin(randAngle0) * glm::cos(randAngle1);
                float dk = glm::sin(randAngle1);
                glm::vec3 d_ray2 = glm::vec3(di, dj, dk);
                
                // Intersection hit by L_pd 
                Ray ray2;
                ray2.p0 = hit.intsec_pos;
                ray2.d = d_ray2;
                Intersection hit2 = Intersect(ray2, scene);

                glm::vec3 dif_color = FindColor(hit2, recursion_depth-1, scene); // L(p,d)
                glm::vec4 dif = hit.tri->material->diffuse; // C_diffuse
                float nd = glm::dot(hit.surface_normal, d_ray2); // n.d 

                glm::vec3 l_seen = nd * glm::vec4(dif.x * dif_color.x, dif.y * dif_color.y, dif.z * dif_color.z);
                            
                return l_seen;
            } else { //specular
                // reflection vector r
                float twoNV = 2 * glm::dot(hit.surface_normal, hit.inr_d);
                glm::vec3 r =  twoNV * hit.surface_normal - hit.inr_d;

                Ray ray_ref;
                ray_ref.p0 = hit.intsec_pos;
                ray_ref.d = r;
                Intersection hit_ref = Intersect(ray_ref, scene);

                glm::vec3 ref_color = FindColor(hit_ref, recursion_depth - 1, scene);
                glm::vec4 spec = hit.tri->material->specular;    
                glm::vec3 l_seen = glm::vec3(spec.x * ref_color.x, spec.y * ref_color.y, spec.z * ref_color.z);
                
                return l_seen;
            }
        }

        */

        //find distance
        if (hit.dist < INF_DIST) {
            if (!(hit.tri->material)) {
                std::cout << "material NULL pointer";
            }
            
            glm::vec4 color = hit.tri->material->emision;

            for(std::pair<std::string,Light*> entry: scene->light) {
                Light* light = entry.second;
                
                // transform light position to camera coord
                glm::vec4 cLightPos = scene->camera->view * light->position;

                // l vector
                glm::vec3 l = normalize(glm::vec3(cLightPos) - cLightPos.w * hit.intsec_pos);

                // v vector = -p
                glm::vec3 v = normalize(-1.0f * hit.intsec_pos);

                // h vector
                glm::vec3 h = normalize(v + l);

                glm::vec4 C = hit.tri->material->ambient;

                float diffsca = std::max(glm::dot(hit.surface_normal,l), 0.0f);
                C += diffsca * hit.tri->material->diffuse;
                
                float specsca = pow(std::max(glm::dot(hit.surface_normal,h), 0.0f),  hit.tri->material->shininess);
                C += specsca * hit.tri->material->specular;
                
                color += glm::vec4(light->color.x * C.x, light->color.y * C.y, light->color.z * C.z, light->color.w * C.w);
            }

            return glm::vec3(color);
        }

        return glm::vec3(0.0f,0.0f,0.0f);
    }; //page 15
};