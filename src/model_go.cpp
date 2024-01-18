//
// Created by Marharyta Haichuk on 18/01/2024.
//
#include <project/gameObject/model_go.h>

namespace KhEngine
{

    ModelGameObject::ModelGameObject(Model &model, ShaderPath shaderPath) : GameObject(shaderPath.vertexShaderPath, shaderPath.fragmentShaderPath),model(model)
    {
    }

    ModelGameObject::ModelGameObject(Model &model, Shader &shader) : GameObject(shader),model(model)
    {
    }

    ModelGameObject::ModelGameObject(const char *modelPath, Shader &shader) : GameObject(shader),model(modelPath)
    {
    }

    ModelGameObject::ModelGameObject(const char *modelPath, ShaderPath shaderPath) : GameObject(shaderPath.vertexShaderPath, shaderPath.fragmentShaderPath),
                                                                                     model(modelPath)
    {
    }

    void ModelGameObject::setModelView(glm::mat4 value) {
        customModel = true;
        modelMtx = value;
    }

    void ModelGameObject::Draw(Shader &shader) {
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

    void ModelGameObject::Destroy() {
        model.Destroy();
    }

    GoblinGameObject::GoblinGameObject(Shader &shader) : ModelGameObject("resources/models/Resault/LitleGoblen.obj", shader)
    {
        originTransform.Scale = glm::vec3(10.0f);
    }

    TerrainGameObject::TerrainGameObject(Shader &shader) : ModelGameObject("resources/models/Scene/Scene.obj", shader)
    {
        originTransform.Position = glm::vec3(0.0f,29.0f,0.0f);
    }
}