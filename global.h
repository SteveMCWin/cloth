#ifndef GLOBAL_H
#define GLOBAL_H

#include "glad.h"
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Global{
public:

    // static glm::mat4 view;
    static glm::mat4 projection;
    static glm::vec3 gravity;
    static const unsigned int cloth_cols = 10;
    static const unsigned int cloth_rows = 10;
    static constexpr float subdivision_length = 0.2f;

};

#endif // !GLOBAL_H
