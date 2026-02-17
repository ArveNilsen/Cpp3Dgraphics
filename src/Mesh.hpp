#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>

#include "Shader.hpp"

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture
{
    std::uint32_t id;
    std::string type;
    std::string path;
};

class Mesh
{
public:
    std::vector<Vertex>         vertices_;
    std::vector<std::uint32_t>  indices_;
    std::vector<Texture>        textures_;

    std::uint32_t VAO;

    Mesh(std::vector<Vertex> vertices, 
            std::vector<std::uint32_t> indices,
            std::vector<Texture> textures);

    void Draw(Shader &shader);
private:
    std::uint32_t VBO, EBO;

    void setupMesh();
};
