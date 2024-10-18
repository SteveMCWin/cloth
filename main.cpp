#include "glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "shader.h"
#include "cloth_vertex.h"
#include "cloth_renderer.h"
#include "cloth_handler.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void printVec3(glm::vec3 vec);

const unsigned int SCR_WIDTH    = 1600;
const unsigned int SCR_HEIGHT   =  900;

float delta_time = 0.0f;
float last_frame = 0.0f;

int main(int, char**){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);

    // vblank_mode=0 ./my_opengl_project 

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Stevica :D", NULL, NULL);
    if(window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if(glfwGetWindowAttrib(window, GLFW_TRANSPARENT_FRAMEBUFFER)){
        std::cout << "The framebuffer is transparrent!" << std::endl;
    }

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_MULTISAMPLE);


    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    }; 

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    glBindVertexArray(0); 

    Shader test_shader = Shader("/home/stevica/openGL_projects/cloth/shaders/v_test.glsl",
                                "/home/stevica/openGL_projects/cloth/shaders/f_test.glsl");
    Shader cloth_vertex_shader = Shader("/home/stevica/openGL_projects/cloth/shaders/v_cloth_vertex.glsl",
                                        "/home/stevica/openGL_projects/cloth/shaders/f_cloth_vertex.glsl");

    glm::vec3 cloth_vertex_positions[10][10];
    float masses[10][10];

    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            cloth_vertex_positions[i][j] = glm::vec3(0.2f * (j-5), 0.2f * (i-5), -1.0f);
            masses[i][j] = 1.0f;
        }
    }

    ClothHandler handler = ClothHandler(cloth_vertex_positions, masses);
    ClothRenderer renderer;

    // for(int i = 0; i < 10; i++){
    //     for(int j = 0; j < 10; j++){
    //         printVec3(handler.cloth_vertices[i][j].position);
    //         std::cout << "{}"
    //     }
    //     std::cout << std::endl << std::endl;
    // }

    // renderer.RenderVertices(handler, cloth_vertex_shader);

    while(!glfwWindowShouldClose(window)){

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT);

        // test_shader.use();
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3);

        renderer.RenderVertices(handler, cloth_vertex_shader);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

void printVec3(glm::vec3 vec){
    std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}" << std::endl;
}

