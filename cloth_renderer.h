#ifndef CLOTH_RENDERER_H
#define CLOTH_RENDERER_H

#include "cloth_handler.h"
#include "cloth_vertex.h"
#include "shader.h"
#include "global.h"

class ClothRenderer {
public:
     ClothRenderer();   // handles all cloth rendering related stuff, the constructor just sets up the vbo and vao
    ~ClothRenderer();

    void RenderCloth(ClothHandler& cloth, Shader& shader);
    void RenderVertices(ClothHandler& cloth, Shader& shader);    // used to render only the vertices by rendering a dot on each cloth vertex position
    void RenderSprings(ClothHandler& cloth, Shader& shader);

private:

    void fillVertBuffer(ClothHandler& cloth);   // since cloth vertex positions change every frame, the vbo should get updated as soon as they do
    void setUpRendering(ClothHandler& cloth, Shader& shader);

    unsigned int vertexVBO;
    unsigned int vertexVAO;
    unsigned int vertexEBOs[9];
    unsigned int springEBOs[20];

    unsigned int rowIndices[9][20];
    unsigned int springIndices[20][10];
};

#endif // !CLOTH_RENDERER_H
