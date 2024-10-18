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

    ClothHandler();
    ClothHandler(glm::vec3 positions[10][10], float masses[10][10]);
    void UpdateVertices();
    //void UpdateSprings();

};

#endif // !CLOTH_HANDLER_H
