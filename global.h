#ifndef GLOBAL_H
#define GLOBAL_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))

class Global{
public:

    // static glm::mat4 view;
    static glm::mat4 projection;
    static glm::vec3 gravity;
    static constexpr float cloth_length = 3.0f;
    static const unsigned int cloth_cols = 20;
    static const unsigned int cloth_rows = 25;
    static const unsigned int sphere_sector_count = 4;
    static const unsigned int sphere_stack_count = 2;
    static const float pi;
    static constexpr float subdivision_length = cloth_length/MIN(cloth_cols,cloth_rows);

};

#endif // !GLOBAL_H
