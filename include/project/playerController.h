//
// Created by Marharyta Haichuk on 15/01/2024.
//

#ifndef M_BOSSFIGHT_PLAYERCONTROLLER_H
#define M_BOSSFIGHT_PLAYERCONTROLLER_H

#include <project/gameObject/model_go.h>
#include <ext/matrix_transform.hpp>
#include <SDL.h>
#include <project/iInputController.h>

namespace KhEngine
{
    class PlayerController: public IInputController
    {
    public:
        PlayerController(ModelGameObject& mgo, glm::vec3 objForward);

        void afterTick() override;
        void onMouseEvent() override;
        glm::mat4 getViewMat4() override;
        void setPosition(glm::vec3 pos) override;
        glm::vec3 getPosition() override;
        void keyboardEvents(float deltaTime) override;
        void Destroy() override;
    protected:
        ModelGameObject * mgo;
    private:
        glm::vec3 originForward;
    };
}



#endif //M_BOSSFIGHT_PLAYERCONTROLLER_H
