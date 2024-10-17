#include "cloth_renderer.h"
#include "cloth_vertex.h"

#include <glm/glm.hpp>


ClothRenderer::ClothRenderer(){

    glGenBuffers(1, &this->triangleVBO);
    glGenVertexArrays(1, &this->triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->triangleVBO);
    glBindVertexArray(this->triangleVAO);

}

void ClothRenderer::RenderCloth(ClothVertex **vertices){



}
