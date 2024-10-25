#include "cloth_vertex.h"
#include <glm/detail/type_vec.hpp>

ClothVertex::ClothVertex(){

    this->position = glm::vec3(0.0f);
    this->previous_position = glm::vec3(0.0f);

    this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = 1.0f;

}

ClothVertex::ClothVertex(glm::vec3 pos, float m){

    this->position = pos;
    this->previous_position = this->position;

    this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = m;
}

void ClothVertex::ApplyForce(glm::vec3 force, float delta_time){

    glm::vec3 new_pos = 2.0f * this->position - this->previous_position + force * delta_time * delta_time;
    this->previous_position = this->position;
    this->position = new_pos;
}
