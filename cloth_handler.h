#ifndef CLOTH_HANDLER_H
#define CLOTH_HANDLER_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

#include "cloth_vertex.h"
//#include "cloth_spring.h"

class ClothHander {

public:
    ClothVertex cloth_vertices[10][10];

    ClothHander();
    ClothHander(glm::vec3 **positions, float **masses);
    void UpdateVertices();
    //void UpdateSprings();

};

#endif // !CLOTH_HANDLER_H
