/**
 * One line description of project.
 * Your name goes here.
 *
 * licence header usually goes here.
 */

#ifndef PROJECT_MAIN_HPP
#define PROJECT_MAIN_HPP

// declorations, includes, etc... go here
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include <filesystem>
#include "project/shader.h"
#include "project/camera.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

namespace KhEngine
{
    void loadTexture(GLuint *textureID, std::string path, std::string);
    void flip_surface(SDL_Surface* surface);

    void setCursorMode(SDL_Window* window, int state);
}


#endif // PROJECT_MAIN_HPP
