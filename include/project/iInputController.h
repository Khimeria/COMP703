//
// Created by Marharyta Haichuk on 15/01/2024.
//

#ifndef M_BOSSFIGHT_IINPUTCONTROLLER_H
#define M_BOSSFIGHT_IINPUTCONTROLLER_H

#include <iostream>
#include "iDestroyable.h"
#include <fwd.hpp>
#include <geometric.hpp>
#include <ext/matrix_transform.hpp>
#include <SDL_scancode.h>
#include <SDL_keyboard.h>

namespace KhEngine
{
    class IInputController: public IDestroyable
            {
    public:
        glm::vec3 Right = glm::vec3(1.0f, 0.0f, 0.0f),
                  Up = glm::vec3(0.0f, 1.0f, 0.0f),
                  Forward = glm::vec3(0.0f, 0.0f, -1.0f);
        const glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f),
                  offset = glm::vec3(0.0f, 0.0f, 0.0f),
                  mask = glm::vec3(1.0f,1.0f,1.0f);
        float speed = 4.0f,
              mouseSensitivity = 0.05f,
              yaw = glm::radians(-90.0f),
              pitch = glm::radians(0.0f);

        bool IsBinded = false;

        IInputController* followController;

        void BindTo(KhEngine::IInputController& inputController);;
        void Unbind();;
        void tick(float deltaTime);;

        virtual void keyboardEvents(float deltaTime);
        virtual void afterTick();
        virtual glm::mat4 getViewMat4();
        virtual void setPosition(glm::vec3 pos);;
        virtual glm::vec3 getPosition();;

        void setSpeed(float speed);;
        void setMovementMask(glm::vec3 mask);;
        void bindMouseInput(int &x, int &y);;
    protected:
        glm::vec3 Position = glm::vec3(0.0f);
        int buttonForward = SDL_SCANCODE_W;
        int buttonBackward = SDL_SCANCODE_S;
        int buttonRight = SDL_SCANCODE_D;
        int buttonLeft = SDL_SCANCODE_A;
        int *mouseX, *mouseY;

        virtual void onMouseEvent(){};
    private:
    };
}


#endif //M_BOSSFIGHT_IINPUTCONTROLLER_H
