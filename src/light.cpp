//
// Created by Marharyta Haichuk on 11/01/2024.
//

#include <project//light.h>
#include <ext/matrix_transform.hpp>

namespace KhEngine
{
    LightSourceManager::LightSourceManager()
    {
    }

    DirectLight LightSourceManager::createDirectLightSource(glm::vec3 color,
                                                            glm::vec3 direction,
                                                            glm::vec3 ambient,
                                                            glm::vec3 diffuse,
                                                            glm::vec3 specular)
    {

        directLight->Color = color;
        directLight->Direction = direction;

        directLight->Ambient = ambient;
        directLight->Diffuse = diffuse;
        directLight->Specular = specular;

        return *directLight;
    }

    PointLight LightSourceManager::addPointLightSource(glm::vec3 color,
                                                         glm::vec3 position,
                                                         glm::vec3 ambient,
                                                         glm::vec3 diffuse,
                                                         glm::vec3 specular,
                                                         float constant,
                                                         float linear,
                                                         float quadratic)
    {
        PointLight light{};
        light.ID = pLights.size();
        light.Color = color;
        light.Position = position;

        light.Ambient = ambient;
        light.Diffuse = diffuse;
        light.Specular = specular;

        light.Constant = constant;
        light.Linear = linear;
        light.Quadratic = quadratic;

        pLights.push_back(&light);

        return light;
    }

    SpotLight LightSourceManager::addSpotLightSource(glm::vec3 color,
                                                      glm::vec3 position,
                                                      glm::vec3 direction,
                                                      glm::vec3 ambient,
                                                      glm::vec3 diffuse,
                                                      glm::vec3 specular,
                                                      float constant,
                                                      float linear,
                                                      float quadratic,
                                                      float cutOff,
                                                      float outerCutOf)
    {
        SpotLight light{};
        light.ID = sLights.size();
        light.Color = color;
        light.Position = position;
        light.Direction = direction;

        light.Ambient = ambient;
        light.Diffuse = diffuse;
        light.Specular = specular;

        light.Constant = constant;
        light.Linear = linear;
        light.Quadratic = quadratic;

        light.CutOff = cutOff;
        light.OuterCutOff = outerCutOf;

        sLights.push_back(&light);

        return light;
    }

    void LightSourceManager::Draw(Shader& shader)
    {
        if(directLight != nullptr)
        {
            shader.setVec3("directLight.direction", directLight->Direction);
            std::string name = "directLight";
            setBaseLightToShader(shader, name, directLight);
        }

        for(int i=0; i< pLights.size(); i++)
        {
            auto light = pLights[i];
            std::ostringstream str;
            str << "pointLights[" << i << "]";
            std::string name = str.str();
            setPointLightToShader(shader,name,light);
        }
        for(int i=0; i< sLights.size(); i++)
        {
            auto light = sLights[i];
            std::ostringstream str;
            str << "spotLights[" << i << "]";
            std::string name= str.str();
            setPointLightToShader(shader,name,light);
            shader.setVec3(name+".direction", light->Direction);
            shader.setFloat(name+".cutOff", light->CutOff);
            shader.setFloat(name+".outerCutOff", light->OuterCutOff);
        }
    }

    void LightSourceManager:: setPointLightToShader(Shader& shader, std::string& name, PointLight* light)
    {
        setBaseLightToShader(shader, name, light);
        shader.setVec3(name+".position",  light->Position);
        std::cout <<name<< " "<<std::to_string(shader.ID)<<": " << std::to_string(light->Position.x)<<" "<<std::to_string(light->Position.y)<<" "<<std::to_string(light->Position.z) <<std::endl;
        shader.setFloat(name+".constant",  light->Constant);
        shader.setFloat(name+".linear",    light->Linear);
        shader.setFloat(name+".quadratic", light->Quadratic);
    }

    void LightSourceManager:: setBaseLightToShader(Shader& shader, std::string & name, BaseLightSource* light)
    {
        shader.setVec3(name + ".color",     light->Color);
        shader.setVec3(name + ".ambient",   light->Ambient);
        shader.setVec3(name + ".diffuse",   light->Diffuse); // darken diffuse light a bit
        shader.setVec3(name + ".specular",  light->Specular);
    }

    void LightSourceManager::addDirectLightSource(DirectLight &light) {
        directLight = &light;
    }

    void LightSourceManager::addPointLightSource(PointLight &light) {
        light.ID = pLights.size();
        pLights.push_back(&light);
    }

    void LightSourceManager::addSpotLightSource(SpotLight &light) {
        light.ID = sLights.size();
        sLights.push_back(&light);
    }
}