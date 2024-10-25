#include "spring.h"
#include "cloth_vertex.h"
#include <glm/detail/func_geometric.hpp>

Spring::Spring(){
    this->stiffness = 50.0f;
    this->rest_length = 0.2f;
}

Spring::Spring(float stiff, float rest_len, ClothVertex& v1, ClothVertex& v2){
    this->stiffness = stiff;
    this->rest_length = rest_len;
    this->end_vertices[0] = v1;
    this->end_vertices[1] = v2;
}

// 
glm::vec3 Spring::calculateSpringForce(){
    return -this->stiffness*(glm::normalize(this->end_vertices[1].position - this->end_vertices[0].position)) * // might have to swap end_pos1 and end_pos2
            (glm::length(this->end_vertices[1].position - this->end_vertices[0].position) - this->rest_length);
}

void Spring::ApplyForce(glm::vec3 force, float delta_t){    // maybe the force application is not correct, perhaps try directly using calculateSpringForce here
    this->end_vertices[0].ApplyForce(-force + this->end_vertices[0].mass * glm::vec3(0.0f, -9.81f, 0.0f), delta_t);
    this->end_vertices[1].ApplyForce( force + this->end_vertices[1].mass * glm::vec3(0.0f, -9.81f, 0.0f), delta_t);
}
