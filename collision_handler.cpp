#include "collision_handler.h"
#include "cloth_handler.h"

// CollisionHandler::CollisionHandler(){
//
// }

void CollisionHandler::HandleCollision(ClothHandler& cloth, std::vector<Sphere> &spheres){

    for(Sphere sphere : spheres){
        for(int i = 0; i < Global::cloth_rows; i++){
            for(int j = 0; j < Global::cloth_cols; j++){
                cloth.cloth_position
            }
        }
    }
}
