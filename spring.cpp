#include "spring.h"
#include "global.h"
#include <GLFW/glfw3.h>

Spring::Spring(){
    this->stiffness = 50.0f;
    this->rest_length = 0.2f;
}

Spring::Spring(float stiff, float rest_len, ClothVertex* v1, ClothVertex* v2){
    this->stiffness = stiff;
    this->rest_length = rest_len;
    this->end_vertices[0] = v1;
    this->end_vertices[1] = v2;
}

glm::vec3 Spring::calculateSpringForce(){

    return -this->stiffness*(glm::normalize((this->end_vertices[1])->position - (this->end_vertices[0])->position)) * 
            (glm::length((this->end_vertices[1])->position - (this->end_vertices[0])->position) - this->rest_length);
}

void Spring::AddForce(float delta_t){

    float damping_coef = 0.08f;  // should fiddle with the the damping coeficient
    glm::vec3 damping_force0 = -damping_coef * ((this->end_vertices[0])->position - (this->end_vertices[0])->previous_position) / delta_t;
    glm::vec3 damping_force1 = -damping_coef * ((this->end_vertices[1])->position - (this->end_vertices[1])->previous_position) / delta_t;

    glm::vec3 force = this->calculateSpringForce();

    // (sin(x*y*t), cos(z*t), sin(cos(5*x*y*z))
    glm::vec3 p1 = this->end_vertices[0]->position;
    glm::vec3 p2 = this->end_vertices[1]->position;

    float time = glfwGetTime();
    float wind_strength = 0.03;
    glm::vec3 wind1 = wind_strength * glm::vec3(sin(p1.x * p1.y * time), cos(p1.z * time), sin(cos(9*p1.x*p1.y*p1.z*time)));
    glm::vec3 wind2 = wind_strength * glm::vec3(sin(p2.x * p2.y * time), cos(p2.z * time), sin(cos(9*p2.x*p2.y*p2.z*time)));
    // glm::vec3 wind1 = wind_strength * glm::vec3(sin(p1.x * p1.y + time), cos(p1.z + time), sin(cos(9*p1.x*p1.y*p1.z+time)));
    // glm::vec3 wind2 = wind_strength * glm::vec3(sin(p2.x * p2.y + time), cos(p2.z + time), sin(cos(9*p2.x*p2.y*p2.z+time)));

    // (this->end_vertices[0])->AddForce(-force + damping_force0 + (this->end_vertices[0])->mass * Global::gravity + wind1);
    // (this->end_vertices[1])->AddForce( force + damping_force1 + (this->end_vertices[1])->mass * Global::gravity + wind2);
    (this->end_vertices[0])->AddForce(-force + damping_force0 + (this->end_vertices[0])->mass * Global::gravity);
    (this->end_vertices[1])->AddForce( force + damping_force1 + (this->end_vertices[1])->mass * Global::gravity);
}

