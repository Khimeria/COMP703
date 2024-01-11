//
// Created by Marharyta Haichuk on 11/01/2024.
//

#ifndef M_BOSSFIGHT_LIGHT_H
#define M_BOSSFIGHT_LIGHT_H

#include <vec3.hpp>
#include <glew.h>
#include <SDL.h>

namespace KhEngine
{
    class LightSource
    {
    public:
        LightSource(glm::vec3 position);
        void loadModel();
    private:
        GLuint VAO,VBO,EBO;
        glm::vec3 position;
    };
}

#endif //M_BOSSFIGHT_LIGHT_H
