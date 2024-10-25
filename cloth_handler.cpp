#include "cloth_handler.h"
#include <glm/vec3.hpp>

ClothHandler::ClothHandler(){

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j] = ClothVertex();
        }
    }
    // initialize horizontal springs
    for(int i = 0; i < 90; i++){
        
    }
    // initialize vertical springs
    for(int i = 0; i < 90; i++){
        
    }

}

ClothHandler::ClothHandler(glm::vec3 positions[10][10], float masses[10][10]){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->cloth_vertices[i][j] = ClothVertex(positions[i][j], masses[i][j]);
        }
    }
}
