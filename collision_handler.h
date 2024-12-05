#ifndef COLLISION_HANDLER_H
#define COLLISION_HANDLER_H

#include <vector>

#include "cloth_handler.h"
#include "sphere.h"

class CollisionHandler {

public:
    // CollisionHandler();
    void HandleCollision(ClothHandler &c, std::vector<Sphere*> s, float delta_t);
};

#endif // !COLLISION_HANDLER_H
