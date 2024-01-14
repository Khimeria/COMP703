//
// Created by Marharyta Haichuk on 14/01/2024.
//

#ifndef M_BOSSFIGHT_MODEL_GO_H
#define M_BOSSFIGHT_MODEL_GO_H

#include "game_object.h"
#include "model.h"

namespace KhEngine
{
    struct ShaderPath
    {
        char* vertexShaderPath;
        char* fragmentShaderPath;
    };

    class ModelGameObject: public GameObject
    {
        public:
        float shininess = 64.0f;

        ModelGameObject(Model& model, ShaderPath shaderPath): GameObject(shaderPath.vertexShaderPath, shaderPath.fragmentShaderPath),model(model)
        {
        }

        ModelGameObject(Model& model, Shader& shader): GameObject(shader),model(model)
        {
        }

        ModelGameObject(const char* modelPath, Shader& shader): GameObject(shader),model(modelPath)
        {
        }

        ModelGameObject(const char* modelPath, ShaderPath shaderPath): GameObject(shaderPath.vertexShaderPath, shaderPath.fragmentShaderPath),
                                         model(modelPath)
        {
        }

        void Draw(Shader& shader) override
        {
            glm::mat4 modelMtx = getModelMatrix();

            shader.use();
            shader.setMat4("model", modelMtx);

            glm::mat3 normModel = transpose(inverse(modelMtx));
            shader.setMat3("normalModel", normModel);

            shader.setFloat("material.shininess", shininess);

            this->model.Draw(shader);
        }

        void Destroy() override
        {
        model.Destroy();
        }
    private:
        Model model;
    };
}
#endif //M_BOSSFIGHT_MODEL_GO_H
