#ifndef SPHERE
#define SPHERE

#include <glad/glad.h>
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

    std::vector<float> vertex_positions;
    std::vector<float> vertex_normals;

    unsigned int sphereRowIndices[Global::sphere_stack_count][2*Global::sphere_sector_count+2];

    unsigned int sphereVAO;
    unsigned int sphereVBO;
    unsigned int sphereRowEBOs[Global::sphere_stack_count];
};


#endif // !SPHERE
