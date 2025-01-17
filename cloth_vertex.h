#ifndef CLOTH_VERTEX_H
#define CLOTH_VERTEX_H

#include <glad/glad.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

class ClothVertex {

public:
    ClothVertex();  // the class is supposed to represent a signle vertex
    ClothVertex(glm::vec3 pos, float m);

    glm::vec3 next_position;
    glm::vec3 position;
    glm::vec3 previous_position;
    glm::vec3 force_applied;
    glm::vec3 normal;
    float mass;
    bool is_pinned;

    void CalculateNextPos(float delta_t);
    void UpdatePos();
    void AddForce(glm::vec3 force);
    void ResetForce();

};

#endif // !CLOTH_VERTEX_H
