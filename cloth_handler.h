#ifndef CLOTH_HANDLER_H
#define CLOTH_HANDLER_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

#include "cloth_vertex.h"
//#include "cloth_spring.h"

class ClothHandler {

public:
    ClothVertex cloth_vertices[10][10];
    glm::vec3 cloth_position = glm::vec3(0.0f, 0.0f, -2.0f);

    ClothHandler();
    ClothHandler(glm::vec3 positions[10][10], float masses[10][10]);
    void UpdateVertices();
    //void UpdateSprings();

};

#endif // !CLOTH_HANDLER_H
