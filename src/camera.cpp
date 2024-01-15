//
// Created by Marharyta Haichuk on 08/01/2024.
//
#include "project/camera.h"
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

namespace KhEngine
{

    Camera::Camera(glm::vec3 position, glm::vec3 target)
    {
        this->Position = position;
        this->direction = glm::normalize(position - target);

        Right = glm::normalize(glm::cross(up, direction));
        Up = glm::normalize(glm::cross(direction, Right));
    }
}