#include "cloth_renderer.h"
#include "cloth_handler.h"
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
    
    for(int i = 0; i < Global::cloth_rows-1; i++){ // load indices so we can use triangle strip
        for(int j = 0; j < Global::cloth_cols; j++){
            this->rowIndices[i][2*j]   = Global::cloth_cols*i + j;
            this->rowIndices[i][2*j+1] = Global::cloth_cols*i + j + Global::cloth_cols;
        }
    }

    // horizontal
    // 0 1 2 ... 7 8 9
    // 10 11 12 ... 17 18 19
    //
    // vertical
    // 0 10 20 ... 70 80 90
    // 1 11 21 ... 71 81 91

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            this->horizontalStructuralSpringIndices[i][j] = Global::cloth_cols * i + j;
        }
    }
    for(int i = 0; i < Global::cloth_cols; i++){
        for(int j = 0; j < Global::cloth_rows; j++){
            this->verticalStructuralSpringIndices[i][j] = i + Global::cloth_cols * j;
        }
    }

    int idx = 0;

    // 0 11
    // 1 12
    // 2 13
    // ...
    for(int i = 0; i < Global::cloth_rows-1; i++){
        for(int j = 0; j < Global::cloth_cols-1; j++){
            this->sheerSpringIndices[0][idx++] = Global::cloth_cols*i+j;
            this->sheerSpringIndices[0][idx++] = Global::cloth_cols*i+j+Global::cloth_cols+1;
        }
    }

    idx = 0;

    // 10 1
    // 11 2
    // 12 3
    // ...
    for(int i = 0; i < Global::cloth_rows-1; i++){
        for(int j = 0; j < Global::cloth_cols-1; j++){
            this->sheerSpringIndices[1][idx++] = Global::cloth_cols*i+j+Global::cloth_cols;
            this->sheerSpringIndices[1][idx++] = Global::cloth_cols*i+j+1;
        }
    }

    glGenBuffers(1, &this->vertexVBO);
    glGenBuffers(Global::cloth_rows-1, this->vertexEBOs);
    glGenVertexArrays(1, &this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
    glBindVertexArray(this->vertexVAO);

    // reserve space for the vertex positions and normals in the vbo
    glBufferData(GL_ARRAY_BUFFER, 6 * Global::cloth_rows * Global::cloth_cols * sizeof(float), NULL, GL_DYNAMIC_DRAW);  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * Global::cloth_rows * Global::cloth_cols * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    for(int i = 0; i < Global::cloth_rows-1; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexEBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->rowIndices[i]), this->rowIndices[i], GL_STATIC_DRAW);
    }

    glGenBuffers(Global::cloth_rows + Global::cloth_cols, this->structuralSpringEBOs);

    for(int i = 0; i < Global::cloth_rows; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->structuralSpringEBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->horizontalStructuralSpringIndices[i]), this->horizontalStructuralSpringIndices[i], GL_STATIC_DRAW);
    }
    for(int i = 0; i < Global::cloth_cols; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->structuralSpringEBOs[i + Global::cloth_rows]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->verticalStructuralSpringIndices[i]), this->verticalStructuralSpringIndices[i], GL_STATIC_DRAW);
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
    glDeleteBuffers(Global::cloth_rows-1, this->vertexEBOs);
    glDeleteBuffers(Global::cloth_rows + Global::cloth_cols, this->structuralSpringEBOs);
    glDeleteBuffers(2, this->sheerSpringEBOs);
}

void ClothRenderer::RenderCloth(ClothHandler& cloth, Shader& shader){

    setUpRendering(cloth, shader);

    for(int i = 0; i < Global::cloth_rows-1; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vertexEBOs[i]);
        glDrawElements(GL_TRIANGLE_STRIP, 2 * Global::cloth_cols, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

}

void ClothRenderer::RenderVertices(ClothHandler& cloth, Shader& shader){

    setUpRendering(cloth, shader);

    glDisable(GL_DEPTH_TEST);

    glDrawArrays(GL_POINTS, 0, Global::cloth_rows * Global::cloth_cols);    // render all 100 points
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}


void ClothRenderer::RenderSprings(ClothHandler& cloth, Shader& shader){

    setUpRendering(cloth, shader);

    glDisable(GL_DEPTH_TEST);
    for(int i = 0; i < Global::cloth_rows; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->structuralSpringEBOs[i]);
        glDrawElements(GL_LINE_STRIP, Global::cloth_cols, GL_UNSIGNED_INT, 0);
    }
    for(int i = 0; i < Global::cloth_cols; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->structuralSpringEBOs[i+Global::cloth_rows]);
        glDrawElements(GL_LINE_STRIP, Global::cloth_rows, GL_UNSIGNED_INT, 0);
    }
    for(int i = 0; i < 2; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sheerSpringEBOs[i]);
        glDrawElements(GL_LINES, 2 * (Global::cloth_rows-1) * (Global::cloth_cols-1), GL_UNSIGNED_INT, 0);
    }
    glEnable(GL_DEPTH_TEST);
}

void ClothRenderer::fillVertBuffer(ClothHandler& cloth){
    glBindVertexArray(this->vertexVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);

    // positions of each vertex are stored in separate cloth_vertex 
    // objects so the idea is to load all of them into one array
    // so they could be loaded into the vbo at once
    float vertices_positions[3*Global::cloth_rows*Global::cloth_cols];
    float vertices_normals[3*Global::cloth_rows*Global::cloth_cols];

    for(int i = 0; i < Global::cloth_rows; i++){
        for(int j = 0; j < Global::cloth_cols; j++){
            // vertex positions
            float vertex_pos[3] = {cloth.cloth_vertices[i][j].position.x, cloth.cloth_vertices[i][j].position.y, cloth.cloth_vertices[i][j].position.z};
            vertices_positions[3*Global::cloth_cols*i+3*j  ] = vertex_pos[0];
            vertices_positions[3*Global::cloth_cols*i+3*j+1] = vertex_pos[1];
            vertices_positions[3*Global::cloth_cols*i+3*j+2] = vertex_pos[2];   // if things don't work, it's probably this
            // vertex normals
            // glm::vec3 n = this->CalculateVertexNormal(cloth, i, j);
            // float vertex_normal[3] = {n.x, n.y, n.z};
            float vertex_normal[3] = {cloth.cloth_vertices[i][j].normal.x, cloth.cloth_vertices[i][j].normal.y, cloth.cloth_vertices[i][j].normal.z};
            vertices_normals[3*Global::cloth_cols*i+3*j  ] = vertex_normal[0];
            vertices_normals[3*Global::cloth_cols*i+3*j+1] = vertex_normal[1];
            vertices_normals[3*Global::cloth_cols*i+3*j+2] = vertex_normal[2];
        }
    }
    
    // for(int i = 0; i < Global::cloth_rows; i++){
    //     for(int j = 0; j < Global::cloth_cols; j++){
    //         float vertex[3] = {}
    //     }
    // }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_positions), vertices_positions);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_positions), sizeof(vertices_normals), vertices_normals);



    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void ClothRenderer::setUpRendering(ClothHandler& cloth, Shader& shader){

    fillVertBuffer(cloth);  // note: very wastefull atm if rendering cloth and springs and points at the same time

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, cloth.cloth_position);
    // glm::mat4 view = glm::mat4(1.0f);
    // glm::mat4 projection = glm::mat4(1.0f);

    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));  // the view matrix will have to be initialized in main if I want camera movement 

    shader.use();
    // shader.setMat4("view", view);
    shader.setMat4("model", model);

    glBindVertexArray(this->vertexVAO);

}

glm::vec3 ClothRenderer::CalculateVertexNormal(ClothHandler& cloth, int i, int j){
    
    int x_dir = (j == Global::cloth_cols-1) ? -1 : 1;
    int y_dir = (i == Global::cloth_rows-1) ? -1 : 1;

    glm::vec3 v1 = cloth.cloth_vertices[i+y_dir][j].position - cloth.cloth_vertices[i][j].position;
    glm::vec3 v2 = cloth.cloth_vertices[i][j+y_dir].position - cloth.cloth_vertices[i][j].position;
    if(x_dir < 0) v1 *= -1.0f;
    // if(y_dir < 0) v2 *= -1.0f;
    return glm::normalize(glm::cross(v1, v2));
}

// glm::vec3 ClothRenderer::CalculateVertexNormal(ClothHandler& cloth, int i, int j){
//     
//     glm::vec3 v1;
//     glm::vec3 v2;
//
//     glm::vec3 norm = glm::vec3(0.0f);
//     int div_counter = 0;
//
//     if(i-1 >= 0){
//         if(j-1 >=0){
//             v1 = cloth.cloth_vertices[i-1][j].position - cloth.cloth_vertices[i][j].position;
//             v2 = cloth.cloth_vertices[i][j-1].position - cloth.cloth_vertices[i][j].position;
//             norm += glm::normalize(glm::cross(v1, v2));
//             div_counter++;
//         }
//         if(j+1 < Global::cloth_cols){
//             v1 = cloth.cloth_vertices[i-1][j].position - cloth.cloth_vertices[i][j].position;
//             v2 = cloth.cloth_vertices[i][j+1].position - cloth.cloth_vertices[i][j].position;
//             norm += glm::normalize(glm::cross(v1, v2));
//             div_counter++;
//         }
//     }
//     if(i+1 < Global::cloth_rows){
//         if(j-1 >=0){
//             v1 = cloth.cloth_vertices[i+1][j].position - cloth.cloth_vertices[i][j].position;
//             v2 = cloth.cloth_vertices[i][j-1].position - cloth.cloth_vertices[i][j].position;
//             norm += glm::normalize(glm::cross(v1, v2));
//             div_counter++;
//         }
//         if(j+1 < Global::cloth_cols){
//             v1 = cloth.cloth_vertices[i+1][j].position - cloth.cloth_vertices[i][j].position;
//             v2 = cloth.cloth_vertices[i][j+1].position - cloth.cloth_vertices[i][j].position;
//             norm += glm::normalize(glm::cross(v1, v2));
//             div_counter++;
//         }
//     }
//
//     if(div_counter)
//         norm = norm/static_cast<float>(div_counter);
//
//     return norm;
// }
