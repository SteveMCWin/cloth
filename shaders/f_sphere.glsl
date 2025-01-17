#version 430
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 sphereColor;

void main() {
    // ambient
    // float ambientStrength = 0.0;
    // vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // vec3 result = (ambient + diffuse + specular) * sphereColor;
    vec3 result = (diffuse + specular) * sphereColor;

    FragColor = vec4(result, 1.0);
}
