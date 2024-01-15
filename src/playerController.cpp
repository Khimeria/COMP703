//
// Created by Marharyta Haichuk on 15/01/2024.
//

#include <project/playerController.h>


namespace KhEngine
{
    PlayerController::PlayerController(ModelGameObject& mgo, glm::vec3 objForward)
    {
        this->mgo = &mgo;
        this->direction = objForward;

        Right = glm::normalize(glm::cross(up, direction));
        Up = glm::normalize(glm::cross(direction, Right));
        Forward = glm::normalize(direction);
    }

    glm::mat4 PlayerController::getViewMat4()
    {
        return glm::lookAt(mgo->transform.Position, mgo->transform.Position + mask * Forward, Up);
    }

    void PlayerController::setPosition(glm::vec3 pos)
    {
        mgo->transform.Position = pos;
    }

    glm::vec3 PlayerController::getPosition()
    {
        return mgo->transform.Position;
    }

    void PlayerController::tick(float deltaTime)
    {
        IInputController::tick(deltaTime);

        auto model = getViewMat4();
        model = glm::scale(model, mgo->originTransform.Scale);
        mgo->setModelView(model);
    }

}
