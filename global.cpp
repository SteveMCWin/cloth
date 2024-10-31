#include "global.h"

glm::vec3 Global::gravity = glm::vec3(0.0f, -9.81f, 0.0f);
glm::mat4 Global::projection = glm::perspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 100.0f);
