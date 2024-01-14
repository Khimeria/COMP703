//
// Created by Marharyta Haichuk on 13/01/2024.
//

#ifndef M_BOSSFIGHT_GAME_OBJECT_H
#define M_BOSSFIGHT_GAME_OBJECT_H

#include <project/shader.h>
#include <project/buffer.h>
#include <vec3.hpp>
#include "IDestroyable.h"

namespace KhEngine
{

    class GameObject: public IDestroyable
    {
    public:
        glm::mat4 view, projection;
        Shader* shader;

        GameObject(char* vertexShaderPath,char* fragmentShaderPath): shader(BufferManager::GetShader(vertexShaderPath,fragmentShaderPath))
        {
            typeHash = typeid(this).hash_code();
        }

        glm::vec3 Position = glm::vec3(0.0f), Scale = glm::vec3(1.0f), Rotation = glm::vec3(0.0f), Color = glm::vec3 (1.0f);
        virtual void Draw() {};
        void Destroy() override {};
    protected:
        unsigned long typeHash;
    };

} // KhEngine

#endif //M_BOSSFIGHT_GAME_OBJECT_H
