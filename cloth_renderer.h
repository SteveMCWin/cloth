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
    glm::vec3 CalculateVertexNormal(ClothHandler& cloth, int i, int j);

    unsigned int vertexVBO;
    unsigned int vertexVAO;
    unsigned int vertexEBOs[Global::cloth_rows-1];
    unsigned int structuralSpringEBOs[Global::cloth_rows + Global::cloth_cols];
    unsigned int sheerSpringEBOs[2];

    unsigned int rowIndices[Global::cloth_rows-1][Global::cloth_cols*2];
    unsigned int horizontalStructuralSpringIndices[Global::cloth_rows][Global::cloth_cols];
    unsigned int verticalStructuralSpringIndices[Global::cloth_cols][Global::cloth_rows];
    unsigned int sheerSpringIndices[2][2*(Global::cloth_rows-1)*(Global::cloth_cols-1)];
};

#endif // !CLOTH_RENDERER_H
