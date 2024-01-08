//
// Created by Marharyta Haichuk on 08/01/2024.
//

#ifndef M_BOSSFIGHT_CAMERA_H
#define M_BOSSFIGHT_CAMERA_H
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace KhEngine
{
    class Camera
    {
    public:
        Camera(glm::vec3 position, glm::vec3 direction);
    private:
        glm::vec3 position;
        glm::vec3 direction;
    };
}
#endif //M_BOSSFIGHT_CAMERA_H
