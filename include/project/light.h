//
// Created by Marharyta Haichuk on 11/01/2024.
//

#ifndef M_BOSSFIGHT_LIGHT_H
#define M_BOSSFIGHT_LIGHT_H

#include <vec3.hpp>
#include <glew.h>
#include <SDL.h>
#include "shader.h"
#include "iDestroyable.h"

namespace KhEngine
{
    struct BaseLightSource
    {
        glm::vec3 Color;

        glm::vec3 Ambient;
        glm::vec3 Diffuse;
        glm::vec3 Specular;
    };

    struct DirectLight: BaseLightSource
    {
        glm::vec3 Direction;
    };


    struct PointLight : BaseLightSource
    {
        unsigned long ID;
        glm::vec3 Position;

        float Constant;
        float Linear;
        float Quadratic;
    };

    struct SpotLight: PointLight
    {
        glm::vec3 Direction;
        float CutOff;
        float OuterCutOff;
    };

    class LightSourceManager: IDestroyable
    {
    public:
        LightSourceManager();
        void Draw(Shader &shader);

        DirectLight createDirectLightSource(glm::vec3 color, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse,
                                            glm::vec3 specular);

        PointLight addPointLightSource(glm::vec3 color, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse,
                                         glm::vec3 specular,
                                         float constant, float linear, float quadratic);

        SpotLight addSpotLightSource(glm::vec3 color, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient,
                                glm::vec3 diffuse,
                                glm::vec3 specular, float constant, float linear, float quadratic, float cutOff,
                                float outerCutOf);

        void addDirectLightSource(DirectLight &light);

        void addPointLightSource(PointLight &light);

        void addSpotLightSource(SpotLight &light);

        void Destroy() override;

    private:
        std::vector<PointLight*> pLights;
        std::vector<SpotLight*> sLights;
        DirectLight* directLight ;

        glm::vec3 color, position = glm::vec3(0.0f), scale = glm::vec3(0.5f, 0.5f, 0.5f);

        void setPointLightToShader(Shader &shader, std::string &name, PointLight* light);

        void setBaseLightToShader(Shader &shader, std::string &name, BaseLightSource* light);
    };
}

#endif //M_BOSSFIGHT_LIGHT_H
