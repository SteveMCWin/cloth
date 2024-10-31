#ifndef CLOTH_HANDLER_H
#define CLOTH_HANDLER_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <cmath>

#include "cloth_vertex.h"
#include "spring.h"
#include "global.h"

class ClothHandler {    // used to store and work with cloth vertices

public:
    ClothVertex cloth_vertices[Global::cloth_rows][Global::cloth_cols];

    Spring      horizontal_structural_springs[Global::cloth_rows][Global::cloth_cols-1];
    Spring      vertical_structural_springs[Global::cloth_rows-1][Global::cloth_cols];
    Spring      falling_shear_springs[Global::cloth_rows-1][Global::cloth_cols-1];
    Spring      rising_shear_springs[Global::cloth_rows-1][Global::cloth_cols-1];

    glm::vec3 cloth_position = glm::vec3(0.0f, 0.0f, -2.0f);

    ClothHandler(glm::vec3 positions[Global::cloth_rows][Global::cloth_cols], float masses[Global::cloth_rows][Global::cloth_cols], float spring_stiffness, float spring_rest_len);
    void UpdateVertices(float delta_t);  // i think the idea should be that we update all the vertices seemingly at once, so if one particle of the spring moves the other won't see it instantly 
    void PinVertices(glm::vec2 v1, glm::vec2 v2);

};

#endif // !CLOTH_HANDLER_H
