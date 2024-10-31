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

    // horizontal
    // 0 1 2 ... 7 8 9
    // 10 11 12 ... 17 18 19
    //
    // vertical
    // 0 10 20 ... 70 80 90
    // 1 11 21 ... 71 81 91

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            this->structuralSpringIndices[i][j] = 10 * i + j;
            this->structuralSpringIndices[i+10][j] = i + 10 * j;
        }
    }

    int idx = 0;

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            this->sheerSpringIndices[0][idx++] = 10*i+j;
            this->sheerSpringIndices[0][idx++] = 10*i+j+11;
        }
    }

    idx = 0;

    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            this->sheerSpringIndices[1][idx++] = 10*i+j+10;
            this->sheerSpringIndices[1][idx++] = 10*i+j+1;
        }
    }

    glGenBuffers(1, &this->vertexVBO);
    glGenBuffers(9, this->vertexEBOs);
    glGenVertexArrays(1, &this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBindVertexArray(this->vertexVAO);

    glBufferData(GL_ARRAY_BUFFER, 300 * sizeof(float), NULL, GL_DYNAMIC_DRAW);  // just reserve space for the vertex positions in the vbo
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    for(int i = 0; i < 9; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexEBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->rowIndices[i]), this->rowIndices[i], GL_STATIC_DRAW);
    }

    glGenBuffers(20, this->structuralSpringEBOs);

    for(int i = 0; i < 20; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->structuralSpringEBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->structuralSpringIndices[i]), this->structuralSpringIndices[i], GL_STATIC_DRAW);
    }

    glGenBuffers(2, this->sheerSpringEBOs);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sheerSpringEBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->sheerSpringIndices[0]), this->sheerSpringIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sheerSpringEBOs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->sheerSpringIndices[1]), this->sheerSpringIndices[1], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

ClothRenderer::~ClothRenderer(){
    glDeleteVertexArrays(1, &this->vertexVAO);
    glDeleteBuffers(1, &this->vertexVBO);
}

void ClothRenderer::RenderCloth(ClothHandler& cloth, Shader& shader){

    setUpRendering(cloth, shader);

    for(int i = 0; i < 9; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexEBOs[i]);
        glDrawElements(GL_TRIANGLE_STRIP, 20, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

}

void ClothRenderer::RenderVertices(ClothHandler& cloth, Shader& shader){

    setUpRendering(cloth, shader);

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_POINTS, 0, 100);    // render all 100 points
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}


void ClothRenderer::RenderSprings(ClothHandler& cloth, Shader& shader){

    setUpRendering(cloth, shader);

    glDisable(GL_DEPTH_TEST);
    for(int i = 0; i < 20; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->structuralSpringEBOs[i]);
        glDrawElements(GL_LINE_STRIP, 10, GL_UNSIGNED_INT, 0);
    }
    for(int i = 0; i < 2; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sheerSpringEBOs[i]);
        glDrawElements(GL_LINES, 162, GL_UNSIGNED_INT, 0);
    }
    glEnable(GL_DEPTH_TEST);
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
            float vertex[3] = {cloth.cloth_vertices[i][j].position.x, cloth.cloth_vertices[i][j].position.y, cloth.cloth_vertices[i][j].position.z};
            vertices_positions[30*i+3*j  ] = vertex[0];
            vertices_positions[30*i+3*j+1] = vertex[1];
            vertices_positions[30*i+3*j+2] = vertex[2];
        }
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_positions), vertices_positions);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ClothRenderer::setUpRendering(ClothHandler& cloth, Shader& shader){

    fillVertBuffer(cloth);  // note: very wastefull atm if rendering cloth and springs and points at the same time

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // the view matrix will have to be initialized in main if I want camera movement 

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    glBindVertexArray(this->vertexVAO);

}


