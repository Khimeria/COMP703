//
// Created by Marharyta Haichuk on 13/01/2024.
//

#ifndef M_BOSSFIGHT_PRIMITIVE_GO_H
#define M_BOSSFIGHT_PRIMITIVE_GO_H

#include "game_object.h"
#include "project/shader.h"
#include <glew.h>
#include "project/renderManager.h"

namespace KhEngine
{
    class Cube: public GameObject
    {
    public:
        Cube();
        void Draw(Shader& shader) override;
    private:
        GLuint VAO = 0, VBO = 0,EBO = 0;
        void loadModel();
    };
}
#endif //M_BOSSFIGHT_PRIMITIVE_GO_H
