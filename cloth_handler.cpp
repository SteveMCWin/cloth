#include "cloth_handler.h"
#include <glm/vec3.hpp>
#include "spring.h"

ClothHandler::ClothHandler(){

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j] = ClothVertex();
        }
    }
    // initialize horizontal springs
    
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 9; j++){
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
            horizontal_structural_springs[i][j] = Spring(1.0f, 0.2f, &this->cloth_vertices[i][j], &this->cloth_vertices[i][j+1]);
        }
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 10; j++){
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
            vertical_structural_springs[i][j] = Spring(1.0f, 0.2f, &this->cloth_vertices[i][j], &this->cloth_vertices[i+1][j]);
        }
    }

}

ClothHandler::ClothHandler(glm::vec3 positions[10][10], float masses[10][10], float spring_stiffness, float spring_rest_len){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j] = ClothVertex(positions[i][j], masses[i][j]);
        }
    }

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 9; j++){
            horizontal_structural_springs[i][j] = Spring(spring_stiffness, spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i][j+1]);
        }
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 10; j++){
            vertical_structural_springs[i][j] = Spring(spring_stiffness, spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i+1][j]);
        }
    }
}

void ClothHandler::UpdateVertices(float delta_t){

    glm::vec3 h_spring_forces[10][9];
    glm::vec3 v_spring_forces[9][10];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 9; j++){
            h_spring_forces[i][j] = horizontal_structural_springs[i][j].calculateSpringForce();
            v_spring_forces[j][i] = vertical_structural_springs[j][i].calculateSpringForce();
        }
    }
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 9; j++){
            glm::vec3 total_force = h_spring_forces[i][j];// + this->cloth_vertices[0][0].mass * glm::vec3(0.0f, -9.8f, 0.0f); // should add damping and make gravity vec global
            horizontal_structural_springs[i][j].ApplyForce(total_force/2.0f, delta_t);
        }
    }
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 10; j++){
            glm::vec3 total_force = v_spring_forces[i][j];// + this->cloth_vertices[0][0].mass * glm::vec3(0.0f, -9.8f, 0.0f); // should add damping and make gravity vec global
            vertical_structural_springs[i][j].ApplyForce(total_force/2.0f, delta_t);
        }
    }
}

void ClothHandler::PinVertices(glm::vec2 v1, glm::vec2 v2){
    cloth_vertices[(int)v1.x][(int)v1.y].is_pinned = true;
    cloth_vertices[(int)v2.x][(int)v2.y].is_pinned = true;
}





