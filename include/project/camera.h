//
// Created by Marharyta Haichuk on 08/01/2024.
//

#ifndef M_BOSSFIGHT_CAMERA_H
#define M_BOSSFIGHT_CAMERA_H
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL_events.h>
#include "iInputController.h"
#include "iDestroyable.h"

namespace KhEngine
{
    class Camera: public IInputController, public IDestroyable
    {
    public:
        Camera(glm::vec3 position, glm::vec3 target);
        void Destroy() override;
    private:
    };
}
#endif //M_BOSSFIGHT_CAMERA_H
