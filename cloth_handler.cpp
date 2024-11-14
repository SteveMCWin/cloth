#include "cloth_handler.h"
#include <glm/vec3.hpp>
#include "spring.h"

ClothHandler::ClothHandler(glm::vec3 positions[Global::cloth_rows][Global::cloth_cols], float masses[Global::cloth_rows][Global::cloth_cols],
                           float spring_stiffness, float spring_rest_len){
    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
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
    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols-1; j++){
            horizontal_structural_springs[i][j] = Spring(spring_stiffness, spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i][j+1]);
        }
    }

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols-3; j++){
            horizontal_bend_springs[i][j] = Spring(3 * spring_stiffness, 3 * spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i][j+3]);
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
    for(int i = 0; i < Global::cloth_rows-1; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            vertical_structural_springs[i][j] = Spring(spring_stiffness, spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i+1][j]);
        }
    }

    for(int i = 0; i < Global::cloth_rows-3; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            vertical_bend_springs[i][j] = Spring(3 * spring_stiffness, 3 * spring_rest_len, &this->cloth_vertices[i][j], &this->cloth_vertices[i+3][j]);
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
    for(int i = 0; i < Global::cloth_rows-1; i++){
        for(int j = 0; j < Global::cloth_cols-1; j++){
            falling_shear_springs[i][j] = Spring(spring_stiffness/10.0f, spring_rest_len * sqrt(2.0), &this->cloth_vertices[i][j], &this->cloth_vertices[i+1][j+1]);
            rising_shear_springs[i][j] = Spring(spring_stiffness/10.0f, spring_rest_len * sqrt(2.0), &this->cloth_vertices[i+1][j], &this->cloth_vertices[i][j+1]);
        }
    }
}

void ClothHandler::UpdateVertices(float delta_t){

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            this->cloth_vertices[i][j].ResetForce();
        }
    }

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols-1; j++){
            this->horizontal_structural_springs[i][j].AddForce(delta_t);
        }
    }

    // for(int i = 0; i < Global::cloth_rows; i++){
    //     for(int j = 0; j < Global::cloth_cols-3; j++){
    //         this->horizontal_bend_springs[i][j].AddForce(delta_t);
    //     }
    // }

    for(int i = 0; i < Global::cloth_rows-1; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            this->vertical_structural_springs[i][j].AddForce(delta_t);
        }
    }

    // for(int i = 0; i < Global::cloth_rows-3; i++){
    //     for(int j = 0; j < Global::cloth_cols; j++){
    //         this->vertical_bend_springs[i][j].AddForce(delta_t);
    //     }
    // }

    for(int i = 0; i < Global::cloth_rows-1; i++){
        for(int j = 0; j < Global::cloth_cols-1; j++){
            this->falling_shear_springs[i][j].AddForce(delta_t);
            this->rising_shear_springs[i][j].AddForce(delta_t);
        }
    }

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            this->cloth_vertices[i][j].ApplyForce(delta_t);
        }
    }

    UpdateVertexNormals();
}

void ClothHandler::UpdateVertexNormals(){

    glm::vec3 v1;
    glm::vec3 v2;

    glm::vec3 normal;

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            // if(i-1 >= 0){
            //     if(j-1 >=0){
            //         v1 = this->cloth_vertices[i-1][j].position - this->cloth_vertices[i][j].position;
            //         v2 = this->cloth_vertices[i][j-1].position - this->cloth_vertices[i][j].position;
            //         normal = glm::normalize(glm::cross(v1, v2));
            //         this->cloth_vertices[i][j].normal += normal;
            //         this->cloth_vertices[i-1][j].normal += normal;
            //         this->cloth_vertices[i][j-1].normal += normal;
            //     }
            //     if(j+1 < Global::cloth_cols){
            //         v1 = this->cloth_vertices[i-1][j].position - this->cloth_vertices[i][j].position;
            //         v2 = this->cloth_vertices[i][j+1].position - this->cloth_vertices[i][j].position;
            //         normal = glm::normalize(glm::cross(v1, v2));
            //         this->cloth_vertices[i][j].normal += normal;
            //         this->cloth_vertices[i-1][j].normal += normal;
            //         this->cloth_vertices[i][j+1].normal += normal;
            //     }
            // }
            if(i+1 < Global::cloth_rows){
                // if(j-1 >=0){
                //     v1 = this->cloth_vertices[i+1][j].position - this->cloth_vertices[i][j].position;
                //     v2 = this->cloth_vertices[i][j-1].position - this->cloth_vertices[i][j].position;
                //     normal = glm::normalize(glm::cross(v1, v2));
                //     this->cloth_vertices[i][j].normal += normal;
                //     this->cloth_vertices[i+1][j].normal += normal;
                //     this->cloth_vertices[i][j-1].normal += normal;
                // }
                if(j+1 < Global::cloth_cols){
                    v1 = this->cloth_vertices[i+1][j].position - this->cloth_vertices[i][j].position;
                    v2 = this->cloth_vertices[i][j+1].position - this->cloth_vertices[i][j].position;
                    normal = glm::normalize(glm::cross(v1, v2));
                    this->cloth_vertices[i][j].normal += normal;
                    this->cloth_vertices[i+1][j].normal += normal;
                    this->cloth_vertices[i][j+1].normal += normal;
                    // v1 = this->cloth_vertices[i+1][j].position - this->cloth_vertices[i+1][j+1].position;
                    // v2 = this->cloth_vertices[i][j+1].position - this->cloth_vertices[i+1][j+1].position;
                    // normal = glm::normalize(glm::cross(v1, v2));
                    // this->cloth_vertices[i][j].normal += normal;
                    // this->cloth_vertices[i+1][j].normal += normal;
                    // this->cloth_vertices[i][j+1].normal += normal;
                }
            }

        }
    }

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            this->cloth_vertices[i][j].normal = glm::normalize(this->cloth_vertices[i][j].normal);
        }
    }

}

void ClothHandler::PinVertices(glm::vec2 v1, glm::vec2 v2){
    this->cloth_vertices[(int)v1.x][(int)v1.y].is_pinned = true;
    this->cloth_vertices[(int)v2.x][(int)v2.y].is_pinned = true;
}





