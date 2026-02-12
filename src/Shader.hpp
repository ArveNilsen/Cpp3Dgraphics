#pragma once

#include <string>
#include <cassert>

#include <glad/gl.h>
#include <glm/glm.hpp>

class Shader final
{
public:
    Shader() = default;

    void loadShaders(const std::string& vertexPath, const std::string& fragmentPath);

    inline void use() const noexcept {
        assert(ID_ != 0);
        glUseProgram(ID_);
    }
    unsigned int ID_ {};

private:
    void checkCompileErrors(unsigned int shader, std::string_view type) const noexcept;

public:
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID_, name.c_str()), static_cast<int>(value)); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID_, name.c_str()), value); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID_, name.c_str()), value); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const
    { 
        glUniform2fv(glGetUniformLocation(ID_, name.c_str()), 1, &value[0]); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    void setVec2(const std::string &name, float x, float y) const
    { 
        glUniform2f(glGetUniformLocation(ID_, name.c_str()), x, y); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const
    { 
        glUniform3fv(glGetUniformLocation(ID_, name.c_str()), 1, &value[0]); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
        glUniform3f(glGetUniformLocation(ID_, name.c_str()), x, y, z); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const
    { 
        glUniform4fv(glGetUniformLocation(ID_, name.c_str()), 1, &value[0]); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    void setVec4(const std::string &name, float x, float y, float z, float w) const
    { 
        glUniform4f(glGetUniformLocation(ID_, name.c_str()), x, y, z, w); 
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        assert(glGetUniformLocation(ID_, name.c_str()) != -1);
    }
};
