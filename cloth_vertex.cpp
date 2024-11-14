#include "cloth_vertex.h"
#include <glm/detail/type_vec.hpp>
#include <iostream>

ClothVertex::ClothVertex(){

    this->next_position = glm::vec3(0.0f);
    this->position = glm::vec3(0.0f);
    this->previous_position = glm::vec3(0.0f);
    this->is_pinned = false;
    this->force_applied = glm::vec3(0.0f);
    this->normal = glm::vec3(0.0f);

    // this->force_applied = glm::vec3(0.0f, 0.0f, 0.0f);  // change force applied to be 0x -gy 0z when you get global class up and going
    this->mass = 0.005f;

}

ClothVertex::ClothVertex(glm::vec3 pos, float m){

    this->next_position = pos;
    this->position = pos;
    this->previous_position = pos;
    this->is_pinned = false;
    this->mass = m;

    this->force_applied = glm::vec3(0.0f);
    this->normal = glm::vec3(0.0f);
}

void ClothVertex::ApplyForce(float delta_time){

    if(is_pinned) return;

    glm::vec3 new_pos = 2.0f * this->position - this->previous_position + this->force_applied * delta_time * delta_time;
    this->previous_position = this->position;   // maybe don't update the positions instantly here but rather make a different function for that
    this->position = new_pos;
}

void ClothVertex::operator=(const ClothVertex& v){

    this->position = v.position;
    this->previous_position = v.previous_position;
    this->mass = v.mass;
}

void ClothVertex::AddForce(glm::vec3 force){
    this->force_applied += force;
}

void ClothVertex::ResetForce(){
    this->force_applied = glm::vec3(0.0f);
}
