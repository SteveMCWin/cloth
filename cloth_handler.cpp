#include "cloth_handler.h"
#include <glm/vec3.hpp>
#include "spring.h"

ClothHandler::ClothHandler(glm::vec3 positions[10][10], float masses[10][10], float spring_stiffness, float spring_rest_len){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j] = ClothVertex(positions[i][j], masses[i][j]);
        }
    }

    // v00 v01
    // v01 v02
    // ...
    // v07 v08
    // v08 v09
    // v10 v11
    // v11 v12
    // ...
    // v17 v18
    // v18 v19
    // v20 v21
    // v21 v22
    // ...
    // ...
    // ...
    // v97 v98
    // v98 v99
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 9; j++){
            horizontal_structural_springs[i][j] = Spring(spring_stiffness, spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i][j+1]);
        }
    }

    // v00 v10
    // v01 v11
    // ...
    // v08 v18
    // v09 v19
    // v10 v20
    // v11 v21
    // ...
    // v18 v28
    // v19 v29
    // v20 v30
    // ...
    // ...
    // ...
    // v88 v98
    // v89 v99
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 10; j++){
            vertical_structural_springs[i][j] = Spring(spring_stiffness, spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i+1][j]);
        }
    }

    // v00 v11
    // v01 v12
    // v02 v13
    // ...
    // v88 v99
    //
    // v10 v01
    // v11 v02
    // v12 v03
    // ...
    // v98 v89
    //
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            falling_shear_springs[i][j] = Spring(spring_stiffness/10.0f, spring_rest_len * sqrt(2.0), &this->cloth_vertices[i][j], &this->cloth_vertices[i+1][j+1]);
            rising_shear_springs[i][j] = Spring(spring_stiffness/10.0f, spring_rest_len * sqrt(2.0), &this->cloth_vertices[i+1][j], &this->cloth_vertices[i][j+1]);
        }
    }
}

void ClothHandler::UpdateVertices(float delta_t){

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j].ResetForce();
        }
    }

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 9; j++){
            this->horizontal_structural_springs[i][j].AddForce(delta_t);
        }
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 10; j++){
            this->vertical_structural_springs[i][j].AddForce(delta_t);
        }
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            this->falling_shear_springs[i][j].AddForce(delta_t);
            this->rising_shear_springs[i][j].AddForce(delta_t);
        }
    }

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j].ApplyForce(delta_t);
        }
    }
}

void ClothHandler::PinVertices(glm::vec2 v1, glm::vec2 v2){
    cloth_vertices[(int)v1.x][(int)v1.y].is_pinned = true;
    cloth_vertices[(int)v2.x][(int)v2.y].is_pinned = true;
}





