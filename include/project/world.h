//
// Created by Marharyta Haichuk on 14/01/2024.
//

#ifndef M_BOSSFIGHT_WORLD_H
#define M_BOSSFIGHT_WORLD_H

#include <project/camera.h>

namespace KhEngine
{
    class World
    {
    public:
        float fov;
        World(Camera& camera, float screenWidth, float screenHeight, float fov): camera(camera)
        {
            this->screenWidth = screenWidth;
            this->screenHeight = screenHeight;
            this->fov = fov;
        }

        void addGameObject(GameObject& go)
        {
            addShader(go.shader);
            gameObjects.push_back(&go);
        }

        void addDirectLight(DirectLight& light)
        {
            lightManager.addDirectLightSource(light);
            //addLightSource(light);
        }

        void addPointLight(PointLight& light)
        {
            lightManager.addPointLightSource(light);
            //addLightSource(light);
        }

        void addSpotLight(SpotLight& light)
        {
            lightManager.addSpotLightSource(light);
            //addLightSource(light);
        }

        void addLightSource(BaseLightSource& light)
        {

            lights.push_back(&light);
        }

        void addShader(Shader* shader)
        {
            auto shaderExist = shaders.find(shader->ID);
            if(shaderExist == shaders.end())
            {
                shaders.insert({shader->ID, shader});
            }
        }

        void Destroy()
        {
            for(auto obj : gameObjects)
            {
                (*obj).Destroy();
            }
            gameObjects.clear();
            lights.clear();
            shaders.clear();
        }

        void tick(float deltaTime)
        {
            for(auto pair: shaders)
            {
                auto shader = *pair.second;

                shader.use();
                KhEngine::ShaderEx::SetViewMat4(shader, camera.getViewMat4());
                KhEngine::ShaderEx::SetProjectionMat4(shader, getProjection());
                shader.setVec3("viewPos", camera.getPosition());

                lightManager.Draw(shader);
            }

            for(auto go:gameObjects)
            {
                (*go).Draw();
            }
        }

        glm::mat4 getProjection() {
            return glm::perspective(glm::radians(fov), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
        }
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
