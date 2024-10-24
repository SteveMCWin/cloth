#include "cloth_renderer.h"
#include "cloth_vertex.h"

#include <glm/detail/type_mat.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


ClothRenderer::ClothRenderer(){

    //  row indices
    //
    //  0 10  1 11 ...  8 18  9 19
    // 10 20 11 21 ... 18 28 19 29
    // 20 30 21 31 ... 28 38 29 39
    //             ...
    // 80 90 81 91 ... 88 98 89 99
    
    for(int i = 0; i < 9; i++){ // load indices so we can use triangle strip
        for(int j = 0; j < 10; j++){
            this->rowIndices[i][2*j]   = 10*i + j;
            this->rowIndices[i][2*j+1] = 10*i + j + 10;
        }
    }

    // std::cout << "{" << std::endl;
    // for(int i = 0; i < 9; i++){
    //     for(int j = 0; j < 10; j++){
    //         std::cout << rowIndices[i][2*j] << ", " << rowIndices[i][2*j+1] << ", ";
    //     }
    //     std::cout << std::endl;
    // }
    // std::cout << "}" << std::endl;

    glGenBuffers(1, &this->vertexVBO);
    glGenBuffers(9, this->vertexEBO);
    glGenVertexArrays(1, &this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBindVertexArray(this->vertexVAO);

    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), NULL, GL_DYNAMIC_DRAW);  // just reserve space for the vertex positions in the vbo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    for(int i = 0; i < 9; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexEBO[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->rowIndices[i]), this->rowIndices[i], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ClothRenderer::~ClothRenderer(){
    glDeleteVertexArrays(1, &this->vertexVAO);
    glDeleteBuffers(1, &this->vertexVBO);
}

void ClothRenderer::RenderCloth(ClothHandler& cloth, Shader shader){

    setUpRendering(cloth, shader);

    for(int i = 0; i < 9; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexEBO[i]);
        glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

}

void ClothRenderer::RenderVertices(ClothHandler& cloth, Shader shader){

    setUpRendering(cloth, shader);

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


void ClothRenderer::setUpRendering(ClothHandler& cloth, Shader& shader){

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

}


