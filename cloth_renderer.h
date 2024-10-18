#ifndef CLOTH_RENDERER_H
#define CLOTH_RENDERER_H

#include "cloth_handler.h"
#include "cloth_vertex.h"
#include "shader.h"

class ClothRenderer {

public:
     ClothRenderer();
    ~ClothRenderer();

    void RenderCloth(ClothHandler& cloth, Shader shader);
    void RenderVertices(ClothHandler& cloth, Shader shader);

private:

    void fillVertBuffer(ClothHandler& cloth);

    unsigned int vertexVBO;
    unsigned int vertexVAO;
};

#endif // !CLOTH_RENDERER_H
