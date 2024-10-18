#include "cloth_renderer.h"
#include "cloth_vertex.h"

#include <glm/detail/type_mat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


ClothRenderer::ClothRenderer(){

    for(int i = 0; i < 300; i++) this->cloth_vert_idk[i] = 0.0f;

    glGenBuffers(1, &this->triangleVBO);
    glGenVertexArrays(1, &this->triangleVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->triangleVBO);
    glBindVertexArray(this->triangleVAO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(this->cloth_vert_idk), this->cloth_vert_idk, GL_DYNAMIC_DRAW);
    // The idea is that the buffer will be just zeros that get translated
    // to the right position with glm::translate that will take the ClothVertex.position vector
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void ClothRenderer::RenderCloth(ClothHandler& cloth, Shader shader){



}

void ClothRenderer::RenderVertices(ClothHandler& cloth, Shader shader){

    glBindBuffer(GL_ARRAY_BUFFER, this->triangleVBO);
    // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cloth.cloth_vertices), &cloth.cloth_vertices);
    // will have to make a for loop and SubData each position

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // the view matrix will have to be initialized in main if I want camera movement 
    projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);   // will have to change this to use the global class at least to get the aspect ratio

    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            glm::mat4 model = glm::mat4(1.0f);
            glm::vec3 vert_pos = glm::vec3(cloth.cloth_vertices[i][j].position[0], cloth.cloth_vertices[i][j].position[1], cloth.cloth_vertices[i][j].position[2]);
            model = glm::translate(model, vert_pos);
        }
    }


}
