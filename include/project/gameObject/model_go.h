//
// Created by Marharyta Haichuk on 14/01/2024.
//

#ifndef M_BOSSFIGHT_MODEL_GO_H
#define M_BOSSFIGHT_MODEL_GO_H

#include "game_object.h"
#include "project/model.h"

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

        void setModelView(glm::mat4 value){
            customModel = true;
            modelMtx = value;
        }

        void Draw(Shader& shader) override
        {
            shader.use();

            if(customModel)
            {
                shader.setMat4("model", modelMtx);
                customModel = false;
            }
            else
            {
                auto modelMtx = getModelMatrix(glm::mat4(1.0f));
                shader.setMat4("model", modelMtx);
            }

            glm::mat3 normModel = transpose(inverse(modelMtx));
            shader.setMat3("normalModel", normModel);

            shader.setFloat("material.shininess", shininess);

            this->model.Draw(shader);
        }

        void Destroy() override
        {
        model.Destroy();
        }

    protected:
        Model model;
    private:

        bool customModel;
    };

    class GoblinGameObject: public ModelGameObject
    {
    public:
        GoblinGameObject(Shader& shader): ModelGameObject("resources/models/Resault/LitleGoblen.obj", shader)
        {
            originTransform.Scale = glm::vec3(10.0f);
        }
    };

    class TerrainGameObject: public ModelGameObject
    {
    public:
        TerrainGameObject(Shader& shader): ModelGameObject("resources/models/Scene/Scene.obj", shader)
        {
            originTransform.Position = glm::vec3(0.0f,29.0f,0.0f);
        }
    };
}
#endif //M_BOSSFIGHT_MODEL_GO_H
