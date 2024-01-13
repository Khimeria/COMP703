//
// Created by Marharyta Haichuk on 11/01/2024.
//

#ifndef M_BOSSFIGHT_LIGHT_H
#define M_BOSSFIGHT_LIGHT_H

#include <vec3.hpp>
#include <glew.h>
#include <SDL.h>
#include "shader.h"

namespace KhEngine
{
    class LightSource
    {
    public:
        LightSource(glm::vec3 position, glm::vec3 color);
        void loadModel();
        void use();
        void setView(glm::mat4& view);
        void setProjection(glm::mat4& projection);

        glm::vec3 getColor();

        void setPosition(glm::vec3& newPos);
        glm::vec3 getPosition();

        void setColor(glm::vec3 color);

    private:
        GLuint VAO,VBO,EBO;
        glm::vec3 color, position = glm::vec3(0.0f), scale = glm::vec3(0.5f, 0.5f, 0.5f);
        Shader lightingShader;
        glm::mat4 model, view, projection;
    };
}

#endif //M_BOSSFIGHT_LIGHT_H
