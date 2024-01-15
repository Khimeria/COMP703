//
// Created by Marharyta Haichuk on 13/01/2024.
//

#ifndef M_BOSSFIGHT_GAME_OBJECT_H
#define M_BOSSFIGHT_GAME_OBJECT_H

#include <project/shader.h>
#include <project/buffer.h>
#include <ext/matrix_transform.hpp>
#include <project/IDestroyable.h>

namespace KhEngine
{

    class GameObject: public IDestroyable
    {
    public:
        glm::vec3 Position = glm::vec3(0.0f),
                  Scale = glm::vec3(1.0f),
                  Rotation = glm::vec3(0.0f),
                  Color = glm::vec3 (1.0f);
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
        glm::mat4 getModelMatrix() const
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Position); // translate it down so it's at the center of the scene
            model = glm::rotate(model,glm::radians(Rotation.x), glm::vec3(1.0f,0.0f,0.0f));
            model = glm::rotate(model,glm::radians(Rotation.y), glm::vec3(0.0f,1.0f,0.0f));
            model = glm::rotate(model,glm::radians(Rotation.z), glm::vec3(0.0f,0.0f,1.0f));
            model = glm::scale(model, Scale);
            return model;
        }
        virtual void Draw(Shader& shader) {};
    protected:
        unsigned long typeHash;
    private:
        glm::mat4 view,
                  projection;
    };

} // KhEngine

#endif //M_BOSSFIGHT_GAME_OBJECT_H
