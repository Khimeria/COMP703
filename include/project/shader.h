#ifndef SHADER_H
#define SHADER_H

#include <glew.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fwd.hpp>

namespace KhEngine{
    class Shader
    {
        public:
            unsigned int ID;
            Shader(const char* vertexPath, const char* fragmentPath);
            void use();
            void setBool(const std::string &name, bool value) const;
            void setInt(const std::string &name, int value) const;
            void setFloat(const std::string &name, float value) const;

            void setMat4(const std::string &name, glm::mat4 value) const;
            void setMat3(const std::string &name, glm::mat3 value) const;
            void setVec3(const std::string &name, float x, float y, float z) const;
            void setVec3(const std::string &name, glm::vec3 value) const;

    private:

            void checkCompileErrors(unsigned int shader, std::string type);
    };
}

#endif