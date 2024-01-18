//
// Created by Marharyta Haichuk on 14/01/2024.
//
#include "project/renderManager.h"

namespace KhEngine
{
    std::unordered_map<unsigned long ,GLuint> KhEngine::RenderManager::userMap = {};
    std::unordered_map<std::string ,Shader> KhEngine::RenderManager::shaderMap = {};

    bool RenderManager::bufferAlreadyExist(unsigned long typeHash) {
        auto exist = userMap.find(typeHash);
        return exist != userMap.end();
    }

    void RenderManager::GenBuffer(unsigned long typeHash, GLuint *VAO, GLuint *VBO, GLuint *EBO) {
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

    GLuint RenderManager::GetVAO(unsigned long typeHash) {
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

    Shader *RenderManager::GetShader(char *pathVertShader, char *pathFragShader) {
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

    std::string RenderManager::generateShaderID(char *pathVertShader, char *pathFragShader) {
        std::ostringstream stream;
        stream<<pathVertShader<<"&&"<<pathFragShader;
        return stream.str();
    }
}
