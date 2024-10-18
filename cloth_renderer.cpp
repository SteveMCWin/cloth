#include "cloth_renderer.h"
#include "cloth_vertex.h"

#include <glm/detail/type_mat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


ClothRenderer::ClothRenderer(){

    glGenBuffers(1, &this->vertexVBO);
    glGenVertexArrays(1, &this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBindVertexArray(this->vertexVAO);

    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ClothRenderer::~ClothRenderer(){
    glDeleteVertexArrays(1, &this->vertexVAO);
    glDeleteBuffers(1, &this->vertexVBO);
}

void ClothRenderer::RenderCloth(ClothHandler& cloth, Shader shader){



}

void ClothRenderer::RenderVertices(ClothHandler& cloth, Shader shader){

    fillVertBuffer(cloth);

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    shader.use();

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // the view matrix will have to be initialized in main if I want camera movement 
    projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);   // will have to change this to use the global class at least to get the aspect ratio

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    glBindVertexArray(this->vertexVAO);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            glm::mat4 model = glm::mat4(1.0f);
            // glm::vec3 vert_pos = glm::vec3(cloth.cloth_vertices[i][j].position[0], cloth.cloth_vertices[i][j].position[1], cloth.cloth_vertices[i][j].position[2]);
            // model = glm::translate(model, cloth.cloth_position);
            shader.setMat4("model", model);
            glDrawArrays(GL_POINTS, 0, 100);
        }
    }

}


void ClothRenderer::fillVertBuffer(ClothHandler& cloth){
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            float vertex[3] = {cloth.cloth_vertices[i][j].position[0], cloth.cloth_vertices[i][j].position[1], cloth.cloth_vertices[i][j].position[2]};
            std::cout << "{" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "}\n";
            glBufferSubData(GL_ARRAY_BUFFER, (10*i+j)*sizeof(vertex), sizeof(vertex),
                            vertex);
        }
        std::cout << std::endl << std::endl;
    }
}







