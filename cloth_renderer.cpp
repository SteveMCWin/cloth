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

    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), NULL, GL_DYNAMIC_DRAW);  // just reserve space for the vertex positions in the vbo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

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

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);


    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // the view matrix will have to be initialized in main if I want camera movement 
    projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);   // will have to change this to use the global class at least to get the aspect ratio

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    shader.setMat4("model", model);

    glBindVertexArray(this->vertexVAO);

    // the comment below is used to check the contents of the vbo (the data is loaded as expected)

    // float vertices_positions[300];
    // glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_positions), vertices_positions);
    // for(int i = 0; i < 300; i++){
    //     if(i%3 == 0) std::cout << std::endl;
    //     std::cout << vertices_positions[i] << "\t";
    // }
    // std::cout << std::endl;

    glDrawArrays(GL_POINTS, 0, 100);    // render all 100 points
    glBindVertexArray(0);

}


void ClothRenderer::fillVertBuffer(ClothHandler& cloth){
    glBindVertexArray(this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);

    // positions of each vertex are stored in separate cloth_vertex 
    // objects so the idea is to load all of them into one array
    // so they could be loaded into the vbo at once
    float vertices_positions[300];
    
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            float vertex[3] = {cloth.cloth_vertices[i][j].position[0], cloth.cloth_vertices[i][j].position[1], cloth.cloth_vertices[i][j].position[2]};
            vertices_positions[30*i+3*j  ] = vertex[0];
            vertices_positions[30*i+3*j+1] = vertex[1];
            vertices_positions[30*i+3*j+2] = vertex[2];
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_positions), vertices_positions);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}







