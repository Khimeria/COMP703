//
// Created by Marharyta Haichuk on 14/01/2024.
//

#ifndef M_BOSSFIGHT_WORLD_H
#define M_BOSSFIGHT_WORLD_H

#include <project/camera.h>
#include "project/gameObject/game_object.h"
#include "light.h"

namespace KhEngine
{
    class World
    {
    public:
        float fov;
        const glm::vec3 Forward = glm::vec3 (0.0f,0.0f,-1.0f);
        const glm::vec3 Backward = glm::vec3 (0.0f,0.0f,1.0f);
        const glm::vec3 Right = glm::vec3 (1.0f,0.0f,0.0f);
        const glm::vec3 Left = glm::vec3 (-1.0f,0.0f,0.0f);

        World(Camera& camera, float screenWidth, float screenHeight, float fov);

        void addGameObject(GameObject& go);
        void addDirectLight(DirectLight& light);
        void addPointLight(PointLight& light);
        void addSpotLight(SpotLight& light);
        void addShader(Shader* shader);
        void Destroy();
        void tick(float deltaTime);

        glm::mat4 getProjection();
    private:
        float screenWidth, screenHeight;
        Camera& camera;
        glm::mat4 projection, view;
        std::vector<GameObject*> gameObjects;
        std::vector<BaseLightSource*> lights;
        std::unordered_map<unsigned long, Shader*> shaders;
        LightSourceManager lightManager;
    };

}
#endif //M_BOSSFIGHT_WORLD_H
