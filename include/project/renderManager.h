//
// Created by Marharyta Haichuk on 13/01/2024.
//

#ifndef M_BOSSFIGHT_RENDERMANAGER_H
#define M_BOSSFIGHT_RENDERMANAGER_H

#include <string>
#include <glew.h>
#include <unordered_map>
#include <project/shader.h>

namespace KhEngine
{
    class RenderManager
    {
    public:
        static std::unordered_map<unsigned long, GLuint> userMap;
        static std::unordered_map<std::string, Shader> shaderMap;

        static bool bufferAlreadyExist(unsigned long typeHash);
        static void GenBuffer(unsigned long typeHash, GLuint* VAO, GLuint* VBO, GLuint* EBO);
        static GLuint GetVAO(unsigned long typeHash);
        static Shader* GetShader(char *pathVertShader, char *pathFragShader);
        static std::string generateShaderID(char *pathVertShader, char *pathFragShader);
    private:

    };
}

#endif //M_BOSSFIGHT_RENDERMANAGER_H
