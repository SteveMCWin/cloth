#include "sphere.h"
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(glm::vec3 pos, float r){
    this->position = pos;
    this->radius = r;

    float phi = Global::pi/2.0;
    float theta = 0.0f;

    float delta_phi = -Global::pi * (1.0f/Global::sphere_stack_count);
    float delta_theta = 2.0*Global::pi * (1.0f/Global::sphere_sector_count);

    for(int i = 0; i < Global::sphere_stack_count+1; i++){
        theta = 0.0f;

        for(int j = 0; j < Global::sphere_sector_count; j++){

            float x = r * std::cos(phi) * std::cos(theta);
            float y = r * std::sin(phi);
            float z = r * std::cos(phi) * std::sin(theta);

            this->vertex_positions.push_back(x);
            this->vertex_positions.push_back(y);
            this->vertex_positions.push_back(z);

            this->vertex_normals.push_back(x - this->position.x);
            this->vertex_normals.push_back(y - this->position.y);
            this->vertex_normals.push_back(z - this->position.z);

            theta += delta_theta;
        }

        phi += delta_phi;
    }

    for(int i = 0; i < Global::sphere_stack_count; i++){
        for(int j = 0; j < Global::sphere_sector_count+1; j++){
            this->sphereRowIndices[i][2*j  ] = (j%Global::sphere_sector_count) + i*Global::sphere_sector_count;
            this->sphereRowIndices[i][2*j+1] = (j%Global::sphere_sector_count) + (i + 1) * Global::sphere_sector_count;
        }
    }


    glGenBuffers(1, &this->sphereVBO);
    glGenBuffers(Global::sphere_stack_count, this->sphereRowEBOs);
    glGenVertexArrays(1, &this->sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->sphereVBO);
    glBindVertexArray(this->sphereVAO);


    glBufferData(GL_ARRAY_BUFFER, this->vertex_positions.size() * sizeof(float) + this->vertex_normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->vertex_positions.size() * sizeof(float), this->vertex_positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, this->vertex_positions.size() * sizeof(float), this->vertex_normals.size() * sizeof(float), this->vertex_normals.data());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(this->vertex_positions.size() * sizeof(float)));
    glEnableVertexAttribArray(1);


    for(int i = 0; i < Global::sphere_stack_count; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereRowEBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->sphereRowIndices[i]), this->sphereRowIndices[i], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Sphere::~Sphere(){
    glDeleteVertexArrays(1, &this->sphereVAO);
    glDeleteBuffers(1, &this->sphereVBO);
    glDeleteBuffers(Global::sphere_stack_count, this->sphereRowEBOs);
}

void Sphere::RenderSphere(Shader& shader){

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    shader.setMat4("model", model);

    glBindVertexArray(this->sphereVAO);

    for(int i = 0; i < Global::sphere_stack_count; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereRowEBOs[i]);
        glDrawElements(GL_TRIANGLE_STRIP, 2 * Global::sphere_sector_count + 2, GL_UNSIGNED_INT, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

































