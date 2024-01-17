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
        glm::vec3 direction,
                  offset,
                  mask = glm::vec3(1.0f,1.0f,1.0f);
        float speed = 4.0f,
              mouseSensitivity = 0.05f,
              yaw = glm::radians(-90.0f),
              pitch = glm::radians(0.0f);

        int mouseX,
            mouseY;

        bool follow = false;

        IInputController* followController;

        void BindTo(KhEngine::IInputController& inputController, glm::vec3 offset)
        {
            follow = true;
            followController = &inputController;
            this->offset = offset;
        };

        void Unbind()
        {
            follow = false;
            followController = nullptr;
            this->offset = glm::vec3(0.0f);
        };

        virtual void tick(float deltaTime)
        {
            if(follow)
            {
                auto pos = mask * followController->getPosition();
                setPosition(pos-offset);
                return;
            }

            auto movementSpeed = speed * deltaTime;

            SDL_GetRelativeMouseState(&mouseX, &mouseY);

            yaw += (float)mouseX * glm::radians(mouseSensitivity);
            pitch -= (float)mouseY * glm::radians(mouseSensitivity);

            auto r89 = glm::radians(89.0f);

            if (pitch > r89)
                pitch = r89;
            if (pitch < -r89)
                pitch = -r89;

            glm::vec3 rotation;
            rotation.x = cos(yaw) * cos(pitch);
            rotation.y = sin(pitch);
            rotation.z = sin(yaw) * cos(pitch);

            std::cout<<"pitch "<< pitch<<std::endl;
            std::cout<<"rotation "<<rotation.x<<" "<<rotation.y<<" "<<rotation.z<<std::endl;


            onMouseEvent(rotation);

            std::cout<<"forward "<<Forward.x<<" "<<Forward.y<<" "<<Forward.z<<std::endl;

            const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
            if (keyboardState[buttonForward])
                setPosition(getPosition() + mask * Forward * movementSpeed);
            if (keyboardState[buttonBackward])
                setPosition(getPosition() -(mask * Forward * movementSpeed));
            if (keyboardState[buttonRight])
                setPosition(getPosition() + mask * Right * movementSpeed);
            if (keyboardState[buttonLeft])
                setPosition(getPosition() -(mask * Right * movementSpeed));
        };

        virtual void onMouseEvent(glm::vec3 rotation){
            direction = rotation;
            Forward = glm::normalize(direction);
            Right = glm::normalize(glm::cross(up, Forward));
        }

        virtual glm::mat4 getViewMat4()
        {
            if(follow)
            {
                auto mat4 = followController->getViewMat4();
                mat4 = glm::translate(mat4, offset);
                //mat4 = glm::inverse(mat4);
                return mat4;
            }
            return glm::lookAt(getPosition(), getPosition() + Forward, Up);
        }

        virtual void setPosition(glm::vec3 pos)
        {
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
    protected:
        glm::vec3 Position;
        int buttonForward = SDL_SCANCODE_W;
        int buttonBackward = SDL_SCANCODE_S;
        int buttonRight = SDL_SCANCODE_D;
        int buttonLeft = SDL_SCANCODE_A;
    };
}


#endif //M_BOSSFIGHT_IINPUTCONTROLLER_H
