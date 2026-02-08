#include <string>
#include <fstream>
#include <iterator>
#include <print>

#include "Shader.hpp"

constexpr std::size_t INFOLOG_SIZE = 1024;

static std::string readFileToString(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary); // binary mode for portability
    if (!ifs)
        throw std::runtime_error("Could not open file: " + filename);

    std::string content(std::istreambuf_iterator<char>(ifs), {});

    return content;
}

void Shader::loadShaders(const std::string& vertexPath, const std::string& fragmentPath) {
    auto vertexCode = readFileToString(vertexPath);
    auto fragmentCode = readFileToString(fragmentPath);

    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    const char* vs = vertexCode.c_str();
    glShaderSource(vertex, 1, &vs, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fs = fragmentCode.c_str();
    glShaderSource(fragment, 1, &fs, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader program
    ID_ = glCreateProgram();
    glAttachShader(ID_, vertex);
    glAttachShader(ID_, fragment);
    glLinkProgram(ID_);
    checkCompileErrors(ID_, "PROGRAM");

    // cleanup
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(unsigned int shader, std::string_view type) const noexcept {
    int success;
    char infoLog[INFOLOG_SIZE];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, INFOLOG_SIZE, nullptr, infoLog);
            std::println("Shader of type {} failed to compile: {}", type, infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, INFOLOG_SIZE, nullptr, infoLog);
            std::println("Shader linker error: {}", infoLog);
        }
    }
}
