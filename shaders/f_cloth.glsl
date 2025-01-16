#version 430
out vec4 FragColor;

in vec3 vFragPos;
in vec3 vNormal;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 clothColor;

void main() {
    // first check if normal is pointing away from the view pos, if so, reverse the normal
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(vNormal);
    vec3 viewDir = normalize(viewPos - vFragPos);

    norm *= floor(dot(norm, viewDir)) * 2.0 + vec3(1.0); 

    vec3 lightDir = normalize(lightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    float specularStrength = 0.3;
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * clothColor;
    // vec3 result = (ambient + diffuse) * clothColor;

    FragColor = vec4(result, 1.0);
}
