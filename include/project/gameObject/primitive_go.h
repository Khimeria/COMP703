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
        Cube(): GameObject("src/shaders/light/light.vsh","src/shaders/light/light_source.fsh")
        {
            typeHash = typeid(this).hash_code();
            if(!RenderManager::bufferAlreadyExist(typeHash))
            {
                loadModel();
            }
            else
            {
                VAO = RenderManager::GetVAO(typeHash);
            }
        }

        void Draw(Shader& shader) override
        {
            glm::mat4 model = getModelMatrix(modelMtx);
            shader.use();
            shader.setMat4("model", model);
            shader.setVec3("lightColor", color);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        void Destroy() override
        {
            //need implement
        };
    private:
        GLuint VAO = 0, VBO = 0,EBO = 0;

        void loadModel()
        {

                float vertices[] = {
                        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
                        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

                        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
                        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
                        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
                        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

                        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
                        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
                        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
                };

                //int indices[] = {0, 1, 3, 1,2,3};

                RenderManager::GenBuffer(typeHash, &VAO, &VBO, nullptr);

                glBindVertexArray(VAO);
                // load data into vertex buffers
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                // A great thing about structs is that their memory layout is sequential for all its items.
                // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
                // again translates to 3/2 floats which translates to a byte array.
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

                //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
                //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

                // set the vertex attribute pointers
                // vertex Positions
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float ), (void*)0);
                //glEnableVertexAttribArray(1);
                //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float), (void*)(3*sizeof(float)))

        }

    };
}
#endif //M_BOSSFIGHT_PRIMITIVE_GO_H
