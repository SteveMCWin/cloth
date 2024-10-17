#include "cloth_vertex.h"
#include <glm/detail/type_vec.hpp>

ClothVertex::ClothVertex(){

    this->position = glm::vec3(0.0f, 0.0f, -5.0f);
    this->previous_position = this->position;
    this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = 1.0f;

}

ClothVertex::ClothVertex(glm::vec3 pos, float m){
    this->position = pos;
    this->previous_position = this->position;
    this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = m;
}
