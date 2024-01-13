//
// Created by Marharyta Haichuk on 08/01/2024.
//

#ifndef M_BOSSFIGHT_CAMERA_H
#define M_BOSSFIGHT_CAMERA_H
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <SDL_events.h>

namespace KhEngine
{
    class Camera
    {
    public:
        Camera(glm::vec3 position, glm::vec3 target);
        glm::mat4 getViewMat4();
        void tick(float deltaTime);
        void setSpeed(float speed);

        glm::vec3 getPosition();
        glm::vec3 cameraRight,cameraUp,cameraForward = glm::vec3(0.0f, 0.0f, -1.0f);

    private:
        //camera
        glm::vec3 position,direction;
        float cameraSpeed = 0.5f;
        float cameraYaw = glm::radians(-90.0f);
        float cameraPitch = glm::radians(0.0f);

        //mouse
        // Mouse position
        int mouseX, mouseY;
        float mouseSensitivity = 0.05f;
    };
}
#endif //M_BOSSFIGHT_CAMERA_H
