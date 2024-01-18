//
// Created by Marharyta Haichuk on 18/01/2024.
//

#include <project/iInputController.h>

namespace KhEngine
{

    void IInputController::BindTo(IInputController &inputController) {
        IsBinded = true;
        followController = &inputController;
        this->offset = inputController.Position - Position;
    }

    void IInputController::Unbind() {
        IsBinded = false;
        followController = nullptr;
        this->offset = glm::vec3(0.0f);
    }

    void IInputController::tick(float deltaTime) {
        if(IsBinded)
        {
            return;
        }

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

        keyboardEvents(deltaTime);

        afterTick();
    }

    void IInputController::keyboardEvents(float deltaTime) {
        auto movementSpeed = speed * deltaTime;
        const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[buttonForward])
            setPosition(getPosition() + mask * Forward * movementSpeed);
        if (keyboardState[buttonBackward])
            setPosition(getPosition() -(mask * Forward * movementSpeed));
        if (keyboardState[buttonRight])
            setPosition(getPosition() + mask * Right * movementSpeed);
        if (keyboardState[buttonLeft])
            setPosition(getPosition() -(mask * Right * movementSpeed));
    }

    void IInputController::afterTick() {
    }

    glm::mat4 IInputController::getViewMat4() {
        if(IsBinded)
        {
            auto x = glm::dot(offset, followController->Right);
            auto y = glm::dot(offset, followController->Up);
            auto z = glm::dot(offset, -followController->Forward);
            auto mat = glm::lookAt(followController->getPosition() - glm::vec3(x,y,z), followController->getPosition() - offset*up/2.0f,up);
            return mat;
        }
        else
            return glm::lookAt(getPosition(), getPosition() + Forward, Up);
    }

    void IInputController::setPosition(glm::vec3 pos) {
        if(IsBinded)
            return;

        Position = pos;
    }

    glm::vec3 IInputController::getPosition() {
        return Position;
    }

    void IInputController::setSpeed(float speed) {
        this->speed = speed;
    }

    void IInputController::setMovementMask(glm::vec3 mask) {
        this->mask = mask;
    }

    void IInputController::bindMouseInput(int &x, int &y) {
        mouseX = &x;
        mouseY = &y;
    }
}
