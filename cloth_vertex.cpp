#include "cloth_vertex.h"
#include <glm/detail/type_vec.hpp>

ClothVertex::ClothVertex(){

    this->position[0] = 0.0f;
    this->position[1] = 0.0f;
    this->position[2] = -10.0f;
    this->previous_position[0] = 0.0f;
    this->previous_position[1] = 0.0f;
    this->previous_position[2] = -10.0f;
    // this->position = glm::vec3(0.0f, 0.0f, -10.0f);
    // this->previous_position = this->position;
    this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = 1.0f;

}

ClothVertex::ClothVertex(glm::vec3 pos, float m){
    // this->position = pos;
    this->position[0] = pos.x;
    this->position[1] = pos.y;
    this->position[2] = pos.z;
    // this->previous_position = this->position;
    this->previous_position[0] = this->position[0];
    this->previous_position[1] = this->position[1];
    this->previous_position[2] = this->position[2];
    this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = m;
}
