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

        ModelGameObject(Model& model, ShaderPath shaderPath);
        ModelGameObject(Model& model, Shader& shader);
        ModelGameObject(const char* modelPath, Shader& shader);
        ModelGameObject(const char* modelPath, ShaderPath shaderPath);

        void setModelView(glm::mat4 value);
        void Draw(Shader& shader) override;
        void Destroy() override;

    protected:
        Model model;
    private:

        bool customModel;
    };

    class GoblinGameObject: public ModelGameObject
    {
    public:
        GoblinGameObject(Shader& shader);
    };

    class TerrainGameObject: public ModelGameObject
    {
    public:
        TerrainGameObject(Shader& shader);
    };
}
#endif //M_BOSSFIGHT_MODEL_GO_H
