#ifndef CLOTH_RENDERER_H
#define CLOTH_RENDERER_H

#include "cloth_vertex.h"

class ClothRenderer {

public:
     ClothRenderer();
    ~ClothRenderer();

    void RenderCloth(ClothVertex **vertices);

private:

    unsigned int triangleVBO;
    unsigned int triangleVAO;
};

#endif // !CLOTH_RENDERER_H
