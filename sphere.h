#ifndef SPHERE
#define SPHERE

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

#include "global.h"

class Sphere {

public:

    Sphere(glm::vec3 pos, glm::vec3 r);

    glm::vec3 position;
    glm::vec3 radius;

    glm::vec3 top_vertex;
    glm::vec3 bot_vertex;
    glm::vec3 vertex_positions[Global::sphere_stack_count][Global::sphere_sector_count];
    glm::vec3 vertex_normals[Global::sphere_stack_count][Global::sphere_sector_count];
    glm::vec3 top_normal;
    glm::vec3 bot_normal;

};


#endif // !SPHERE
