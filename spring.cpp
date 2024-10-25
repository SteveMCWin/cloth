#include "spring.h"
#include "cloth_vertex.h"
#include <glm/detail/func_geometric.hpp>

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

