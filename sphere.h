#ifndef SPHERE
#define SPHERE

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>
#include <vector>

#include "global.h"
#include "shader.h"

class Sphere {

public:

     Sphere(glm::vec3 pos, float r);
    ~Sphere();
    void RenderSphere(Shader &shader);

    glm::vec3 position;
    float radius;

    // glm::vec3 top_vertex_position;
    // glm::vec3 bot_vertex_position;
    // glm::vec3 top_normal;
    // glm::vec3 bot_normal;
    // glm::vec3 vertex_positions[Global::sphere_stack_count][Global::sphere_sector_count];
    // glm::vec3 vertex_normals[Global::sphere_stack_count][Global::sphere_sector_count];

    // float float_vertex_positions[Global::sphere_stack_count * Global::sphere_sector_count * 3 + 6];
    std::vector<float> float_vertex_positions;
    std::vector<float> float_vertex_normals;

    unsigned int sphereRowIndices[Global::sphere_stack_count-1][2*Global::sphere_sector_count+2];
    unsigned int sphereTopIndices[Global::sphere_sector_count+1];
    unsigned int sphereBotIndices[Global::sphere_sector_count+1];

    unsigned int sphereVAO;
    unsigned int sphereVBO;
    unsigned int sphereRowEBOs[Global::sphere_stack_count-1];
    unsigned int sphereTopEBO;
    unsigned int sphereBotEBO;
};


#endif // !SPHERE
