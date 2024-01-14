//
// Created by Marharyta Haichuk on 14/01/2024.
//
#include <project/buffer.h>

namespace KhEngine
{
    std::unordered_map<unsigned long ,GLuint> KhEngine::BufferManager::userMap = {};
    std::unordered_map<std::string ,Shader> KhEngine::BufferManager::shaderMap = {};
}
