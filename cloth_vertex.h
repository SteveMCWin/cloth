#ifndef CLOTH_VERTEX_H
#define CLOTH_VERTEX_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

class ClothVertex {

public:
    ClothVertex();  // the class is supposed to represent a signle vertex
    ClothVertex(glm::vec3 pos, float m);

    float position[3];
    float previous_position[3];
    glm::vec3 force_applied;
    float mass;

    // so the idea is that the cloth handler will fist calculate the firce from the springs
    // and then pass that force and delta time to this function so the vertices actually move, after they are in the right position
    // the cloth renderer will render the cloth :O
    void ApplyForce(glm::vec3 force, float delta_t);    // calculated using Verlet Integration (i think?)

};

#endif // !CLOTH_VERTEX_H
