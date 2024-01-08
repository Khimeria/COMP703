//
// Created by Marharyta Haichuk on 08/01/2024.
//
#include "project/camera.h"
#include <iostream>

namespace KhEngine
{

    Camera::Camera(glm::vec3 position, glm::vec3 target)
    {
        Camera::position = position;
        Camera::direction = glm::normalize(position - target);

        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, direction));
        glm::vec3 cameraUp = glm::cross(direction, cameraRight);

        glm::mat4 view1 = glm::mat4(1);
        view1 = glm::translate(view1, glm::vec3(position.x,position.y,-position.z));
        glm::mat4 matr = glm::mat4(glm::vec4(cameraRight,0),
                                   glm::vec4(cameraUp,0),
                                   glm::vec4(direction,0),
                                   glm::vec4(0,0,0,1));
        view1 = view1*matr;

        glm::mat4 view2 = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f));
        if(view1 == view2)
            std::cout << "equals" << std::endl;
    }
}