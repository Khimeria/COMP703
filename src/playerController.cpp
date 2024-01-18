//
// Created by Marharyta Haichuk on 15/01/2024.
//

#include <project/playerController.h>


namespace KhEngine
{
    PlayerController::PlayerController(ModelGameObject& mgo, glm::vec3 objForward)
    {
        this->mgo = &mgo;
        originForward = direction = objForward;

        Right = glm::normalize(glm::cross(up, direction));
        Up = glm::normalize(glm::cross(direction, Right));
        Forward = glm::normalize(direction);
    }

    glm::mat4 PlayerController::getViewMat4()
    {
        auto pi = glm::radians(180.0f);
        auto dot = glm::dot(originForward, Forward);
        auto det = glm::dot(Up, glm::cross(originForward, Forward));
        auto alpha = atan2(-det, -dot) + pi;

        auto model = glm::mat4 (1.0f);
        model = glm::translate(model, glm::vec3(getPosition()));
        model = glm::rotate(model, alpha, glm::vec3(0.0f, 1.0f, 0.0f)); // then rotate
        return model;
    }

    void PlayerController::onMouseEvent(){
        IInputController::onMouseEvent();

        //fix forward according to up
        Right = normalize(Right - dot(Right, up) * up);
        Forward = cross(Right, up);
    }

    void PlayerController::setPosition(glm::vec3 pos)
    {
        if(mgo== nullptr)
            return;

        IInputController::setPosition(pos);
        mgo->transform.Position = pos;
    }

    glm::vec3 PlayerController::getPosition()
    {
        if(mgo== nullptr)
            return glm::vec3 (0.0f);
        return mgo->transform.Position;
    }

    void PlayerController::afterTick() {
        auto model = getViewMat4();
        if(mgo== nullptr)
            return;
        model = glm::scale(model, mgo->originTransform.Scale);
        model = glm::scale(model, mgo->transform.Scale);
        mgo->setModelView(model);
    }

    void PlayerController::keyboardEvents(float deltaTime)
    {
        auto movementSpeed = speed * deltaTime;
        const Uint8 *keyboardState = SDL_GetKeyboardState(nullptr);
        if (keyboardState[buttonForward])
            setPosition(getPosition() + mask * Forward * movementSpeed);
        if (keyboardState[buttonBackward])
            setPosition(getPosition() -(mask * Forward * movementSpeed));
        if (keyboardState[buttonRight])
            setPosition(getPosition() - mask * Right * movementSpeed);
        if (keyboardState[buttonLeft])
            setPosition(getPosition() +(mask * Right * movementSpeed));
    }

    void PlayerController::Destroy() {
        this->mgo = nullptr;
    }
}
