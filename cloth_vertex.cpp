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

void ClothVertex::CalculateNextPos(float delta_time){

    if(is_pinned) return;

    this->next_position = 2.0f * this->position - this->previous_position + this->force_applied * delta_time * delta_time;
}

void ClothVertex::UpdatePos(){
    this->previous_position = this->position;
    this->position = this->next_position;
}

void ClothVertex::AddForce(glm::vec3 force){
    this->force_applied += force;
}

void ClothVertex::ResetForce(){
    this->force_applied = glm::vec3(0.0f);
}
