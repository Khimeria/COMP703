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

        static bool bufferAlreadyExist(unsigned long typeHash)
        {
            auto exist = userMap.find(typeHash);
            return exist != userMap.end();
        }
        static void GenBuffer(unsigned long typeHash, GLuint* VAO, GLuint* VBO, GLuint* EBO)
        {
            if(VAO != nullptr)
            {
                glGenVertexArrays(1, VAO);
                RenderManager::userMap.insert({typeHash, *VAO});
            }

            if(VBO != nullptr)
                glGenBuffers(1, VBO);
            if(EBO != nullptr)
                glGenBuffers(1, EBO);

        }
        static GLuint GetVAO(unsigned long typeHash)
        {
            auto exist = userMap.find(typeHash);
            if(exist != userMap.end())
            {
                return exist->second;
            }
            else
            {
                return -1;
            }
        }

        static Shader* GetShader(char *pathVertShader, char *pathFragShader)
        {
            std::string pathId = generateShaderID(pathVertShader, pathFragShader);
            auto exist = shaderMap.find(pathId);
            if(exist != shaderMap.end()){
                return &exist->second;
            }
            else
            {
                Shader shader(pathVertShader,pathFragShader);
                shaderMap.insert({pathId,shader});
                return &shaderMap.find(pathId)->second; //todo:: can i optimise it?
            }
        }

        static std::string generateShaderID(char *pathVertShader, char *pathFragShader)
        {
            std::ostringstream stream;
            stream<<pathVertShader<<"&&"<<pathFragShader;
            return stream.str();
        }

    private:

    };
}

#endif //M_BOSSFIGHT_RENDERMANAGER_H
