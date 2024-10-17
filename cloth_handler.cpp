#include "cloth_handler.h"

ClothHander::ClothHander(){

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            // ClothVertex v;
            this->cloth_vertices[i][j] = ClothVertex();
        }
    }

}

ClothHander::ClothHander(glm::vec3 **positions, float **masses){

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            // ClothVertex v = ClothVertex(positions[i][j], masses[i][j]);
            this->cloth_vertices[i][j] = ClothVertex(positions[i][j], masses[i][j]);
        }
    }
}
