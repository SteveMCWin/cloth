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
    // for(int i = 0; i < 10; i++){
    //     for(int j = 0; j < 10; j++){
    //         glm::mat4 model = glm::mat4(1.0f);
    //         // glm::vec3 vert_pos = glm::vec3(cloth.cloth_vertices[i][j].position[0], cloth.cloth_vertices[i][j].position[1], cloth.cloth_vertices[i][j].position[2]);
    //         // model = glm::translate(model, cloth.cloth_position);
    //         shader.setMat4("model", model);
    //         glDrawArrays(GL_POINTS, 0, 100);
    //     }
    // }
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", model);

    glBindVertexArray(this->vertexVAO);
    glDrawArrays(GL_POINTS, 0, 100);
    glBindVertexArray(0);

}


void ClothRenderer::fillVertBuffer(ClothHandler& cloth){
    glBindVertexArray(this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    float vertices_positions[300];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            float vertex[3] = {cloth.cloth_vertices[i][j].position[0], cloth.cloth_vertices[i][j].position[1], cloth.cloth_vertices[i][j].position[2]};
            vertices_positions[30*i+3*j  ] = vertex[0];
            vertices_positions[30*i+3*j+1] = vertex[1];
            vertices_positions[30*i+3*j+2] = vertex[2];
            // std::cout << 30*i+3*j << std::endl << 30*i+3*j+1 << std::endl << 30*i+3*j+2 << std::endl;
            // std::cout << "{" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << "}\n";
            // glBufferSubData(GL_ARRAY_BUFFER, (10*i+j)*sizeof(vertex), sizeof(vertex), vertex);
        }
        // std::cout << std::endl << std::endl;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_positions), vertices_positions);
    // std::cout << "Before getting data: " << std::endl << std::endl;
    //
    // for(int i = 0; i < 300; i++){
    //     if(i%3 == 0) std::cout << std::endl;
    //     std::cout << vertices_positions[i] << "\t";
    // }
    // 
    // std::cout << "After getting data: " << std::endl << std::endl;
    //
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_positions), vertices_positions);
    // for(int i = 0; i < 300; i++){
    //     if(i%3 == 0) std::cout << std::endl;
    //     std::cout << vertices_positions[i] << "\t";
    // }
    // std::cout << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}







