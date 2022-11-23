#include "RayTracer.h"
#include <glm/glm.hpp>
#include <limits>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#define GLM_SWIZZLE

namespace RayTracer {
    
    void Raytrace(Camera cam, RTScene scene, Image &image){
        int w = image.width; int h = image.height;
        int sample_num = 10;
        int rec = 1;
        float inv_sam = 1 / sample_num;

        for (int j = 0; j < h; j++){
            for (int i = 0; i < w; i++){
                glm::vec3 sum_color = glm::vec3(0.0f, 0.0f, 0.0f);
                
                for (int k = 0; k < sample_num; k++) {
                    float rand_i = i + double(rand()) / RAND_MAX; // [i,i+1]
                    float rand_j = j + double(rand()) / RAND_MAX; // [j,j+1]
                    Ray ray = RayThruPixel(cam, rand_i, rand_j, w, h);
                    Intersection hit = Intersect(ray, scene);
                    sum_color += FindColor(hit, rec, scene);
                }

                image.pixels[j * w + i] = inv_sam * sum_color;
            }
        }
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

    //TODO intersection struct or class? how to initialize?
    Intersection Intersect(Ray ray, Triangle tri) {
        glm::vec3 p1 = tri.pos[0];
        glm::vec3 p2 = tri.pos[1];
        glm::vec3 p3 = tri.pos[2];
        glm::vec3 d = ray.d;
        glm::vec3 p0 = ray.p0;
        
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
            inter.surface_normal = glm::normalize(glm::vec3(lamdaT.x * tri.normal[0] 
                                    + lamdaT.y * tri.normal[1] + lamdaT.z * tri.normal[2]));
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
        Intersection hit;
        
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

    glm::vec3 FindColor(Intersection hit, int recursion_depth, RTScene scene) {
        /*if (recursion_depth == 0) {
            glm::vec3 light_sum = 0.0;
            
            for(std::pair<std::string,Light*> entry: scene.light) {
                Light* light = entry.second;

                // find visibility
                int vis = 1;
                Ray ray;
                ray.p0 = hit.intsec_pos;
                ray.d = light->position - hit.intersec_pos;
                Intersection light_hit = Intersect(ray, scene);
                if (light_hit.dist == std::numeric_limits<float>::max()) {
                    vis = 0;
                }

                // find color
                glm::vec3 l = glm::normalize(hit.intsec_pos.w * light->position.xyz - 
                                        light->position.w * hit.intsec_pos.xyz);
                glm::vec3 light
                light_sum = light_sum + hit.tri->material->diffuse * light->color *
                            max(glm::dot(hit.surface_normal, l), 0) * vis;
            }
            return light_sum / scene.light.size();
        }

        // for (int k = 0; k < sample_num; k++) {
        //     float rand_i = hit.intsec_pos[0] + double(rand()) / RAND_MAX;
        //     float rand_j = hit.intsec_pos[1] + double(rand()) / RAND_MAX;

        float randChoose = double(rand()) / RAND_MAX;
        if (randChoose <= 0.5) {
            //diffuse
            //calculate d: TODO how to make sure d is uniform?
            float randAngle0 = double(rand()) / RAND_MAX * M_PI;
            float randAngle1 = double(rand()) / RAND_MAX * M_PI;
            float di = glm::sin(randAngle0) * glm::sin(randAngle1);
            float dj = glm::sin(randAngle0) * glm::cos(randAngle1);
            float dk = glm::sin(randAngle1);
            glm::vec3 d_ray2 = glm::vec3(di, dj, dk);
            //color seen by L_pd
            Ray ray2;
            ray2.p0 = hit.intsec_pos;
            ray2.d = d_ray2;
            Intersection hit2 = Intersect(ray2, scene);
            glm::vec3 diffuse_color = FindColor(hit, recursion_depth-1, scene);
            //C_diffuse * L(p,d) (n dot d)
            return hit.tri->material->diffuse * diffuse_color * 
                        glm::dot(hit.surface_normal, d)
            //hit.tri->material->diffuse * FindColor()
            //glm::dot(d, hit.surface_normal);
        } else {
            //specular
            glm::vec3 r = 2*glm::dot(hit.surface_normal)
        }
            
        //}
        return 0;*/
    
        //find distance
        if (hit.dist < std::numeric_limits<float>::max()) {
                

            // add emission E first. TODO, keep getting error for material->emision
            // glm::vec4 color = hit.tri->material->emision;
            if (!(hit.tri->material)) {
                std::cout << "material NULL pointer";
            }
            
            glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

            for(std::pair<std::string,Light*> entry: scene.light) {
                Light* light = entry.second;
                // transform light position to camera coord
                glm::vec4 cLightPos = scene.camera->view * light->position;

                // l vector
                glm::vec3 l = normalize(glm::vec3(cLightPos) - cLightPos.w * hit.intsec_pos);

                // v vector = -p
                glm::vec3 v = normalize(-1.0f * hit.intsec_pos);

                // h vector
                glm::vec3 h = normalize(v + l);

                glm::vec4 C = hit.tri->material->ambient;

                glm::vec4 diffuse = hit.tri->material->diffuse;
                float diffsca = std::max(glm::dot(hit.surface_normal,l), 0.0f);
                C += glm::vec4(diffuse.x * diffsca, diffuse.y * diffsca, diffuse.z * diffsca, diffuse.w * diffsca);
                
                glm::vec4 specular = hit.tri->material->specular;
                float specsca = pow(std::max(glm::dot(hit.surface_normal,h), 0.0f),  hit.tri->material->shininess);
                C += glm::vec4(specular.x*specsca, specular.y*specsca, specular.z*specsca, specular.w*specsca);
                
                color += glm::vec4(light->color.x * C.x, light->color.y * C.y, light->color.z * C.z, light->color.w * C.w);
            }

            return color;
        }
        return glm::vec3(0.0f,0.0f,0.0f);
    } //page 15
};