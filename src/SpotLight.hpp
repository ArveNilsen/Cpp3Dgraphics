struct SportLight
{
    glm::vec3 position; // If using vec4 set w to 0.0
    glm::vec3 direction; // If using vec4 set w to 0.0

    float cutOff;
    float outerCutOff;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant; // Usually 1.0
    float linear;
    float quadratic;

    /*
     * float distance    = length(light.position - FragPos);
     * float attenuation = 1.0 / (light.constant + light.linear * 
     *      distance + light.quadratic * (distance * distance));
     * ambient  *= attenuation; 
     * diffuse  *= attenuation;
     * specular *= attenuation;
     */
};
