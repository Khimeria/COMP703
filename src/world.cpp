//
// Created by Marharyta Haichuk on 18/01/2024.
//
#include <project/world.h>

namespace KhEngine
{

    World::World(Camera &camera, float screenWidth, float screenHeight, float fov) : camera(camera)
    {
        this->screenWidth = screenWidth;
        this->screenHeight = screenHeight;
        this->fov = fov;
    }

    glm::mat4 World::getProjection() {
        return glm::perspective(glm::radians(fov), 1.0f * screenWidth / screenHeight, 0.1f, 100.0f);
    }

    void World::tick(float deltaTime) {
        for(auto pair: shaders)
        {
            auto shader = *pair.second;

            shader.use();
            shader.setMat4("view", camera.getViewMat4());
            shader.setMat4("projection", getProjection());
            shader.setVec3("viewPos", camera.getPosition());

            lightManager.Draw(shader);
        }

        for(auto go:gameObjects)
        {
            (*go).Draw();
        }
    }

    void World::addGameObject(GameObject &go) {
        addShader(go.shader);
        gameObjects.push_back(&go);
    }

    void World::Destroy() {
        for(auto obj : gameObjects)
        {
            (*obj).Destroy();
        }
        gameObjects.clear();
        lights.clear();
        shaders.clear();
        lightManager.Destroy();
    }

    void World::addShader(Shader *shader) {
        auto shaderExist = shaders.find(shader->ID);
        if(shaderExist == shaders.end())
        {
            shaders.insert({shader->ID, shader});
        }
    }

    void World::addSpotLight(SpotLight &light) {
        lightManager.addSpotLightSource(light);
    }

    void World::addPointLight(PointLight &light) {
        lightManager.addPointLightSource(light);
    }

    void World::addDirectLight(DirectLight &light) {
        lightManager.addDirectLightSource(light);
    }
}
