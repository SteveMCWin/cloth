#include "collision_handler.h"
#include "glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/detail/type_vec.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "shader.h"
#include "cloth_vertex.h"
#include "cloth_renderer.h"
#include "cloth_handler.h"
#include "camera.h"
#include "sphere.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void printVec3(glm::vec3 vec);

const unsigned int SCR_WIDTH    = 1600;
const unsigned int SCR_HEIGHT   =  900;

float delta_time = 0.0f;
float last_frame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// bool n_pressed = false;
// bool n_released = false;

int main(int, char**){

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

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
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    if(glfwGetWindowAttrib(window, GLFW_TRANSPARENT_FRAMEBUFFER)){
        std::cout << "The framebuffer is transparrent!" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // glEnable(GL_MULTISAMPLE);


    Shader cloth_vertex_shader  = Shader("/home/stevica/openGL_projects/cloth/shaders/v_cloth_vertex.glsl",
                                         "/home/stevica/openGL_projects/cloth/shaders/f_cloth_vertex.glsl");
    Shader spring_shader        = Shader("/home/stevica/openGL_projects/cloth/shaders/v_spring.glsl",
                                         "/home/stevica/openGL_projects/cloth/shaders/f_spring.glsl");
    Shader cloth_shader         = Shader("/home/stevica/openGL_projects/cloth/shaders/v_cloth.glsl",
                                         "/home/stevica/openGL_projects/cloth/shaders/f_cloth.glsl");
    Shader sphere_shader        = Shader("/home/stevica/openGL_projects/cloth/shaders/v_sphere.glsl",
                                         "/home/stevica/openGL_projects/cloth/shaders/f_sphere.glsl");

    cloth_shader.use();
    cloth_shader.setMat4("projection", Global::projection);

    sphere_shader.use();
    sphere_shader.setMat4("projection", Global::projection);

    cloth_vertex_shader.use();
    cloth_vertex_shader.setMat4("projection", Global::projection);

    spring_shader.use();
    spring_shader.setMat4("projection", Global::projection);

    const unsigned int cols = Global::cloth_cols;
    const unsigned int rows = Global::cloth_rows;

    glm::vec3 cloth_vertex_positions[rows][cols];
    float masses[rows][cols];

    for(int i = 0; i < rows; i++){    // Generating position data for each cloth vertex
        for(int j = cols-1; j >= 0; j--){
            // cloth_vertex_positions[i][j] = glm::vec3(Global::subdivision_length * (j-5), Global::subdivision_length * (i-5), -1.0f);
            cloth_vertex_positions[i][j] = glm::vec3(Global::subdivision_length * (j-5), 0.0f, Global::subdivision_length * (i-5)); // not good, use cloth position
            masses[i][j] = 0.008f;
        }
    }

    float spring_stiffness = 1000.0f;

    ClothHandler cloth_handler = ClothHandler(cloth_vertex_positions, masses, spring_stiffness, Global::subdivision_length, glm::vec3(0.0, 1.5f, -4.5f));
    CollisionHandler collision_handler;
    ClothRenderer *renderer = new ClothRenderer();

    cloth_handler.PinVertices(glm::vec2((float)(Global::cloth_rows-1), 0.0f), glm::vec2((float)(Global::cloth_rows-1), (float)(Global::cloth_cols-1)));

    glm::vec3 lightPos = glm::vec3(1.0f, 4.0f, -2.0f);
    glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 clothColor = glm::vec3(0.3f, 0.8f, 0.4f);
    glm::vec3 sphereColor = glm::vec3(0.8f, 0.2f, 0.3f);

    cloth_shader.use();
    cloth_shader.setVec3("lightPos", lightPos);
    cloth_shader.setVec3("lightColor", lightColor);
    cloth_shader.setVec3("clothColor", clothColor);

    sphere_shader.use();
    sphere_shader.setVec3("lightPos", lightPos);
    sphere_shader.setVec3("lightColor", lightColor);
    sphere_shader.setVec3("sphereColor", sphereColor);

    Sphere *s1 = new Sphere(glm::vec3(0.0, -1.0, -3.0), 0.5);
    std::vector<Sphere*> spheres;
    spheres.push_back(s1);

    while(!glfwWindowShouldClose(window)){

        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cloth_shader.use();
        cloth_shader.setMat4("view", camera.GetViewMatrix());
        cloth_shader.setVec3("viewPos", camera.Position);

        sphere_shader.use();
        sphere_shader.setMat4("view", camera.GetViewMatrix());
        sphere_shader.setVec3("viewPos", camera.Position);

        cloth_vertex_shader.use();
        cloth_vertex_shader.setMat4("view", camera.GetViewMatrix());
        spring_shader.use();
        spring_shader.setMat4("view", camera.GetViewMatrix());

        cloth_handler.UpdateVertices(delta_time);
        collision_handler.HandleCollision(cloth_handler, spheres);
        cloth_handler.UpdateVertexNormals();

        renderer->RenderCloth(cloth_handler, cloth_shader);
        // renderer.RenderSprings(handler, spring_shader);
        // renderer.RenderVertices(handler, cloth_vertex_shader);
        for(Sphere* s : spheres){
            s->RenderSphere(sphere_shader);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete renderer;
    for(Sphere* s : spheres){
        delete s;
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
    // if(glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
    //     n_pressed = true;
    // }
    // if(glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE){
    //     n_released = true;
    // }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, delta_time);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, delta_time);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, delta_time);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void printVec3(glm::vec3 vec){
    std::cout << "{" << vec.x << ", " << vec.y << ", " << vec.z << "}" << std::endl;
}

