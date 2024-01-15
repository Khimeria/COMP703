//
// Created by Marharyta Haichuk on 15/01/2024.
//

#ifndef M_BOSSFIGHT_IINPUTCONTROLLER_H
#define M_BOSSFIGHT_IINPUTCONTROLLER_H

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

        bool follow;
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

            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;

            direction.x = cos(yaw) * cos(pitch);
            direction.y = sin(pitch);
            direction.z = sin(yaw) * cos(pitch);
            Forward = glm::normalize(direction);

            Right = glm::normalize(glm::cross(up, Forward));

            const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
            if (keyboardState[SDL_SCANCODE_W])
                setPosition(getPosition() + mask * Forward * movementSpeed);
            if (keyboardState[SDL_SCANCODE_S])
                setPosition(getPosition() -(mask * Forward * movementSpeed));
            if (keyboardState[SDL_SCANCODE_A])
                setPosition(getPosition() + mask * Right * movementSpeed);
            if (keyboardState[SDL_SCANCODE_D])
                setPosition(getPosition() -(mask * Right * movementSpeed));
        };

        virtual glm::mat4 getViewMat4()
        {
            if(follow)
            {
                auto mat4 = followController->getViewMat4();
                mat4 = glm::translate(mat4, offset);
                //mat4 = glm::inverse(mat4);
                return mat4;
            }
            return glm::lookAt(Position, Position + Forward, Up);
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
    };
}


#endif //M_BOSSFIGHT_IINPUTCONTROLLER_H
