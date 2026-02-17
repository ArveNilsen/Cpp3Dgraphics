struct DirectionalLight
{
    glm::vec4 direction; // Use vec3 or set w to 0.0f

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};
