//
// Created by Marharyta Haichuk on 08/01/2024.
//
#include "project/camera.h"
#include <iostream>
#include <SDL.h>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

namespace KhEngine
{

    Camera::Camera(glm::vec3 position, glm::vec3 target)
    {
        Camera::position = position;
        Camera::direction = glm::normalize(position - target);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        cameraRight = glm::normalize(glm::cross(up, direction));
        cameraUp = glm::cross(direction, cameraRight);
    }

    glm::mat4 Camera::getViewMat4()
    {
        return glm::lookAt(position, position+cameraForward, cameraUp);
    }

    void Camera::setSpeed(float speed){
        cameraSpeed = speed;
    }

    void Camera::tick(float deltaTime)
    {
        auto movementSpeed = cameraSpeed * deltaTime;

        const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[SDL_SCANCODE_W])
            position += cameraForward * movementSpeed;
        if (keyboardState[SDL_SCANCODE_S])
           position -= cameraForward * movementSpeed;
        if (keyboardState[SDL_SCANCODE_A])
            position += cameraRight * movementSpeed;
        if (keyboardState[SDL_SCANCODE_D])
            position -= cameraRight * movementSpeed;

    }
}