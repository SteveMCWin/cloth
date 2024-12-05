#include "collision_handler.h"
#include "cloth_handler.h"

// CollisionHandler::CollisionHandler(){
//
// }

void CollisionHandler::HandleCollision(ClothHandler& cloth, std::vector<Sphere*> spheres){

    for(Sphere* sphere : spheres){
        for(int i = 0; i < Global::cloth_rows; i++){
            for(int j = 0; j < Global::cloth_cols; j++){
                float dis_to_sphere = glm::length(cloth.cloth_position + cloth.cloth_vertices[i][j].position - sphere->position);
                if(dis_to_sphere < sphere->radius){
                    glm::vec3 new_pos = sphere->position + sphere->radius * glm::normalize(cloth.cloth_position + cloth.cloth_vertices[i][j].position - sphere->position);
                    cloth.cloth_vertices[i][j].position = new_pos;
                }
            }
        }
    }
}
