//
// Created by Marharyta Haichuk on 18/01/2024.
//
#include <project/gameObject/game_object.h>

namespace KhEngine
{

    GameObject::GameObject(char *vertexShaderPath, char *fragmentShaderPath) : shader(RenderManager::GetShader(vertexShaderPath, fragmentShaderPath))
    {
        typeHash = typeid(this).hash_code();
    }

    GameObject::GameObject(Shader &shader) :shader(&shader){}

    void GameObject::Draw() {
        Draw(*shader);
    }

    glm::mat4 GameObject::applyOriginModelMatrix(glm::mat4 model) const {
        model = glm::translate(model, originTransform.Position); // translate it down so it's at the center of the scene
        model = glm::rotate(model,glm::radians(originTransform.Rotation.x), glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(originTransform.Rotation.y), glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model,glm::radians(originTransform.Rotation.z), glm::vec3(0.0f,0.0f,1.0f));
        model = glm::scale(model, originTransform.Scale);
        return model;
    }

    glm::mat4 GameObject::getModelMatrix(glm::mat4 model) const {
        //glm::mat4 model = glm::mat4(1.0f);
        //model = applyOriginModelMatrix(model);
        model = glm::translate(model, transform.Position + originTransform.Position); // translate it down so it's at the center of the scene
        model = glm::rotate(model,glm::radians(transform.Rotation.x + originTransform.Rotation.x), glm::vec3(1.0f,0.0f,0.0f));
        model = glm::rotate(model,glm::radians(transform.Rotation.y + originTransform.Rotation.y), glm::vec3(0.0f,1.0f,0.0f));
        model = glm::rotate(model,glm::radians(transform.Rotation.z + originTransform.Rotation.z), glm::vec3(0.0f,0.0f,1.0f));
        model = glm::scale(model, transform.Scale * originTransform.Scale);
        return model;
    }
}