#version 430 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;

out vec3 vFragPos;
out vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vFragPos = vec3(model * vec4(vPos, 1.0));
    vNormal = mat3(transpose(inverse(model))) * vNorm;
    gl_Position = projection * view * model * vec4(vPos, 1.0f);
}


