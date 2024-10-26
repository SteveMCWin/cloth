#ifndef CLOTH_HANDLER_H
#define CLOTH_HANDLER_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "cloth_vertex.h"
#include "spring.h"

class ClothHandler {    // used to store and work with cloth vertices

public:
    ClothVertex cloth_vertices[10][10];

    Spring      horizontal_structural_springs[10][9];
    Spring      vertical_structural_springs[9][10];

    glm::vec3 cloth_position = glm::vec3(0.0f, 0.0f, -2.0f);

    ClothHandler();
    ClothHandler(glm::vec3 positions[10][10], float masses[10][10], float spring_stiffness, float spring_rest_len);
    void UpdateVertices(float delta_t);  // i think the idea should be that we update all the vertices seemingly at once, so if one particle of the spring moves the other won't see it instantly 
    void PinVertices(glm::vec2 v1, glm::vec2 v2);

};

#endif // !CLOTH_HANDLER_H
