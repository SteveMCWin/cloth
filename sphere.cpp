#include "sphere.h"
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(glm::vec3 pos, float r){
    this->position = pos;
    this->radius = r;

    // this->top_vertex_position = glm::vec3(pos.x, pos.y + r, pos.z);
    // this->bot_vertex_position = glm::vec3(pos.x, pos.y - r, pos.z);
    // this->top_normal = glm::vec3(0.0,  1.0, 0.0);
    // this->bot_normal = glm::vec3(0.0, -1.0, 0.0);

    float delta_phi = -Global::pi * (1.0f/Global::sphere_stack_count);
    float delta_theta = 2.0*Global::pi * (1.0f/Global::sphere_sector_count);

    // this->float_vertex_positions[0] = pos.x;
    // this->float_vertex_positions[1] = pos.y;
    // this->float_vertex_positions[2] = pos.z;
    // this->float_vertex_positions.push_back(pos.x);
    // this->float_vertex_positions.push_back(pos.y+r);
    // this->float_vertex_positions.push_back(pos.z);
    this->float_vertex_positions.push_back(0.0);
    this->float_vertex_positions.push_back(r);
    this->float_vertex_positions.push_back(0.0);

    this->float_vertex_normals.push_back(0.0f);
    this->float_vertex_normals.push_back(1.0f);
    this->float_vertex_normals.push_back(0.0f);

    for(float phi = Global::pi/2.0 + delta_phi; phi >= -Global::pi/2.0; phi += delta_phi) {
        for(float theta = 0.0; theta < delta_theta * Global::sphere_sector_count; theta += delta_theta) {

            float x = r * std::cos(phi) * std::cos(theta);
            float y = r * std::sin(phi);
            float z = r * std::cos(phi) * std::sin(theta);

            this->float_vertex_positions.push_back(x);
            this->float_vertex_positions.push_back(y);
            this->float_vertex_positions.push_back(z);

            this->float_vertex_normals.push_back(x - this->position.x);
            this->float_vertex_normals.push_back(y - this->position.y);
            this->float_vertex_normals.push_back(z - this->position.z);
        }
    }

    // this->float_vertex_positions.push_back(pos.x);
    // this->float_vertex_positions.push_back(pos.y-r);
    // this->float_vertex_positions.push_back(pos.z);
    this->float_vertex_positions.push_back(0.0);
    this->float_vertex_positions.push_back(-r);
    this->float_vertex_positions.push_back(0.0);

    this->float_vertex_normals.push_back( 0.0f);
    this->float_vertex_normals.push_back(-1.0f);
    this->float_vertex_normals.push_back( 0.0f);

    std::cout << "num of elements in float_vertex_positions: " << this->float_vertex_positions.size() << "\nwhich is this many vec3s: " << this->float_vertex_positions.size()/3 << std::endl;

    int x = 0;
    // for(float f : this->float_vertex_positions){
    //     if(x%3 == 0) std::cout << "<";
    //     std::cout << f;
    //     if(x%3 != 2) std::cout << ", ";
    //     else if(x%3 == 2) std::cout << ">" << "\t";
    //     x++;
    //     if(x%15 == 0) std::cout <<  std::endl;
    // }

    unsigned int i = 0;
    unsigned int j = 0;

    // we assume that the veretex position data will start with the top vertex and end with the bot vertex
    for(i = 0; i < Global::sphere_sector_count + 1; i++){
        this->sphereTopIndices[i] = i;
        this->sphereBotIndices[i] = Global::sphere_sector_count * Global::sphere_stack_count + 1 - i;
    }

    // 1 11
    // 2 12
    // 3 13
    // ...
    // 10 20
    // 1 11
    //
    // 11 21
    // 12 22
    // ...
    // 20 30
    // 11 21
    //

    for(i = 0; i < Global::sphere_stack_count-1; i++){
        for(j = 0; j < Global::sphere_sector_count+1; j++){
            this->sphereRowIndices[i][2*j  ] = (j%Global::sphere_sector_count)+1 + i*Global::sphere_sector_count;
            this->sphereRowIndices[i][2*j+1] = (j%Global::sphere_sector_count)+1 + (i + 1) * Global::sphere_sector_count;
        }
    }


    glGenBuffers(1, &this->sphereVBO);
    glGenBuffers(1, &this->sphereTopEBO);
    glGenBuffers(1, &this->sphereBotEBO);
    glGenBuffers(Global::sphere_stack_count-1, this->sphereRowEBOs);
    glGenVertexArrays(1, &this->sphereVAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->sphereVBO);
    glBindVertexArray(this->sphereVAO);


    glBufferData(GL_ARRAY_BUFFER, this->float_vertex_positions.size() * sizeof(float) + this->float_vertex_normals.size() * sizeof(float), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, this->float_vertex_positions.size() * sizeof(float), this->float_vertex_positions.data());
    glBufferSubData(GL_ARRAY_BUFFER, this->float_vertex_positions.size() * sizeof(float), this->float_vertex_normals.size() * sizeof(float), this->float_vertex_normals.data());
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(this->float_vertex_positions.size() * sizeof(float)));
    glEnableVertexAttribArray(1);


    for(i = 0; i < Global::sphere_stack_count-1; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereRowEBOs[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->sphereRowIndices[i]), this->sphereRowIndices[i], GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereTopEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->sphereTopEBO), this->sphereTopIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereBotEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(this->sphereBotEBO), this->sphereBotIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

Sphere::~Sphere(){
    glDeleteVertexArrays(1, &this->sphereVAO);
    glDeleteBuffers(1, &this->sphereVBO);
    glDeleteBuffers(Global::sphere_stack_count-1, this->sphereRowEBOs);
    glDeleteBuffers(1, &this->sphereTopEBO);
    glDeleteBuffers(1, &this->sphereBotEBO);
}

void Sphere::RenderSphere(Shader& shader){

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->position);
    shader.setMat4("model", model);

    glBindVertexArray(this->sphereVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereTopEBO);
    glDrawElements(GL_TRIANGLE_FAN, Global::sphere_sector_count+1, GL_UNSIGNED_INT, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereBotEBO);
    glDrawElements(GL_TRIANGLE_FAN, Global::sphere_sector_count+1, GL_UNSIGNED_INT, 0);

    for(int i = 0; i < Global::sphere_stack_count-1; i++){
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->sphereRowEBOs[i]);
        glDrawElements(GL_TRIANGLE_STRIP, 2 * Global::sphere_sector_count + 2, GL_UNSIGNED_INT, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

































