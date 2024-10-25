#ifndef CLOTH_HANDLER_H
#define CLOTH_HANDLER_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

#include "cloth_vertex.h"
#include "spring.h"

class ClothHandler {    // used to store and work with cloth vertices

public:
    ClothVertex cloth_vertices[10][10];
    Spring      structural_springs[2][90];    // not sure if I should make this a 2d array, one dimension for horisontal and other for vertical springs

    glm::vec3 cloth_position = glm::vec3(0.0f, 0.0f, -2.0f);

    ClothHandler();
    ClothHandler(glm::vec3 positions[10][10], float masses[10][10]);
    void UpdateVertices();  // i think the idea should be that we update all the vertices seemingly at once, so if one particle of the spring moves the other won't see it instantly 
    //void UpdateSprings();

};

#endif // !CLOTH_HANDLER_H
