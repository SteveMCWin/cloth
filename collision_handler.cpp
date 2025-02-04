#include "collision_handler.h"
#include "cloth_handler.h"

// CollisionHandler::CollisionHandler(){
//
// }

void CollisionHandler::HandleCollision(ClothHandler& cloth, std::vector<Sphere*> spheres, float delta_t){

    for(Sphere* sphere : spheres){
        for(int i = 0; i < Global::cloth_rows; i++){
            for(int j = 0; j < Global::cloth_cols; j++){
                float dis_to_sphere = glm::length(cloth.cloth_position + cloth.cloth_vertices[i][j].next_position - sphere->position);
                if(dis_to_sphere < sphere->radius+0.03){
                    // cloth.cloth_vertices[i][j].AddForce(glm::normalize(cloth.cloth_position + cloth.cloth_vertices[i][j].next_position - sphere->position)*30.0f);
                    // cloth.cloth_vertices[i][j].CalculateNextPos(delta_t);
                    
                    // following line is for infinite friction but I don't think it would work for moving objects
                    cloth.cloth_vertices[i][j].next_position = cloth.cloth_vertices[i][j].position;
                }
            }
        }
    }
}
