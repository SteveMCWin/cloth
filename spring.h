#ifndef SPRING_H
#define SPRING_H

#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>

#include "cloth_vertex.h"

class Spring {

public:
    Spring();
    Spring(float stiff, float rest_len, ClothVertex* v1, ClothVertex* v2);

    glm::vec3 calculateSpringForce();
    void AddForce(float delta_t);

    // void operator=(const Spring& s);

private:
    float stiffness;
    float rest_length;
    ClothVertex *end_vertices[2];

};

#endif // !SPRING_H
