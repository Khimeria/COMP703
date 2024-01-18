//
// Created by Marharyta Haichuk on 13/01/2024.
//

#ifndef M_BOSSFIGHT_GAME_OBJECT_H
#define M_BOSSFIGHT_GAME_OBJECT_H

#include "project/shader.h"
#include "project/renderManager.h"
#include <ext/matrix_transform.hpp>
#include <project/iDestroyable.h>

namespace KhEngine
{
    struct Transform
    {
        glm::vec3 Position = glm::vec3(0.0f);
        glm::vec3 Scale = glm::vec3(1.0f);
        glm::vec3 Rotation = glm::vec3(0.0f);
        float width;
        float height;
    };

    class GameObject: public IDestroyable
    {
    public:
        Transform transform,
                  originTransform;
        glm::vec3 color = glm::vec3 (1.0f);
        Shader*   shader;

        GameObject(Shader& shader):shader(&shader)
        {
        }
        GameObject(char* vertexShaderPath,char* fragmentShaderPath): shader(RenderManager::GetShader(vertexShaderPath, fragmentShaderPath))
        {
            typeHash = typeid(this).hash_code();
        }
        void Draw()
        {
            Draw(*shader);
        }

        glm::mat4 applyOriginModelMatrix(glm::mat4 model) const
        {
            model = glm::translate(model, originTransform.Position); // translate it down so it's at the center of the scene
            model = glm::rotate(model,glm::radians(originTransform.Rotation.x), glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model,glm::radians(originTransform.Rotation.y), glm::vec3(0.0f,1.0f,0.0f));
            model = glm::rotate(model,glm::radians(originTransform.Rotation.z), glm::vec3(0.0f,0.0f,1.0f));
            model = glm::scale(model, originTransform.Scale);
            return model;
        }

        glm::mat4 getModelMatrix(glm::mat4 model) const
        {
            //glm::mat4 model = glm::mat4(1.0f);
            //model = applyOriginModelMatrix(model);
            model = glm::translate(model, transform.Position + originTransform.Position); // translate it down so it's at the center of the scene
            model = glm::rotate(model,glm::radians(transform.Rotation.x + originTransform.Rotation.x), glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model,glm::radians(transform.Rotation.y + originTransform.Rotation.y), glm::vec3(0.0f,1.0f,0.0f));
            model = glm::rotate(model,glm::radians(transform.Rotation.z + originTransform.Rotation.z), glm::vec3(0.0f,0.0f,1.0f));
            model = glm::scale(model, transform.Scale * originTransform.Scale);
            return model;
        }
        virtual void Draw(Shader& shader) {};
    protected:
        glm::mat4 modelMtx = glm::mat4(1.0f);
        unsigned long typeHash;
    private:
        glm::mat4 view,
                  projection;
    };

} // KhEngine

#endif //M_BOSSFIGHT_GAME_OBJECT_H
