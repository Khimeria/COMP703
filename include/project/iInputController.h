//
// Created by Marharyta Haichuk on 15/01/2024.
//

#ifndef M_BOSSFIGHT_IINPUTCONTROLLER_H
#define M_BOSSFIGHT_IINPUTCONTROLLER_H

#include <iostream>

namespace KhEngine
{
    class IInputController {
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

        void BindTo(KhEngine::IInputController& inputController)
        {
            IsBinded = true;
            followController = &inputController;
            this->offset = inputController.Position - Position;
        };

        void Unbind()
        {
            IsBinded = false;
            followController = nullptr;
            this->offset = glm::vec3(0.0f);
        };

        void tick(float deltaTime)
        {
            if(IsBinded)
            {
                return;
            }

            auto movementSpeed = speed * deltaTime;

            yaw += (float)*mouseX * glm::radians(mouseSensitivity);
            pitch -= (float)*mouseY * glm::radians(mouseSensitivity);

            auto r89 = glm::radians(89.0f);

            if (pitch > r89)
                pitch = r89;
            if (pitch < -r89)
                pitch = -r89;

            direction.x = cos(yaw) * cos(pitch);
            direction.y = sin(pitch);
            direction.z = sin(yaw) * cos(pitch);

            Forward = glm::normalize(direction);
            Right = glm::normalize(glm::cross(up, Forward));

            onMouseEvent();

            const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
            if (keyboardState[buttonForward])
                setPosition(getPosition() + mask * Forward * movementSpeed);
            if (keyboardState[buttonBackward])
                setPosition(getPosition() -(mask * Forward * movementSpeed));
            if (keyboardState[buttonRight])
                setPosition(getPosition() + mask * Right * movementSpeed);
            if (keyboardState[buttonLeft])
                setPosition(getPosition() -(mask * Right * movementSpeed));

            afterTick();
        };

        virtual void afterTick(){
        }

        virtual glm::mat4 getViewMat4()
        {
            if(IsBinded)
            {
                auto x = glm::dot(offset, followController->Right);
                auto y = glm::dot(offset, followController->Up);
                auto z = glm::dot(offset, -followController->Forward);
                auto mat = glm::lookAt(followController->getPosition() - glm::vec3(x,y,z), followController->getPosition() + followController->Forward,up);
                return mat;
            }
            else
                return glm::lookAt(getPosition(), getPosition() + Forward, Up);
        }

        virtual void setPosition(glm::vec3 pos)
        {
            if(IsBinded)
                return;

            Position = pos;
        };

        virtual glm::vec3 getPosition(){
            return Position;
        };
        void setSpeed(float speed)
        {
            this->speed = speed;
        };
        void setMovementMask(glm::vec3 mask)
        {
            this->mask = mask;
        };
        void bindMouseInput(int &x, int &y)
        {
            mouseX = &x;
            mouseY = &y;
        };
    protected:
        glm::vec3 Position = glm::vec3(0.0f);
        int buttonForward = SDL_SCANCODE_W;
        int buttonBackward = SDL_SCANCODE_S;
        int buttonRight = SDL_SCANCODE_D;
        int buttonLeft = SDL_SCANCODE_A;
        int *mouseX, *mouseY;

        virtual void onMouseEvent(){
        }

    private:
    };
}


#endif //M_BOSSFIGHT_IINPUTCONTROLLER_H
