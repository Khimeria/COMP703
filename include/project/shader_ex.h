//
// Created by Marharyta Haichuk on 14/01/2024.
//

#ifndef M_BOSSFIGHT_SHADER_EX_H
#define M_BOSSFIGHT_SHADER_EX_H

#include <ext/matrix_float4x4.hpp>
#include <project/shader.h>

namespace KhEngine
{
    class ShaderEx
    {
    public:
        static void SetProjectionMat4(Shader &shader, glm::mat4 projection){
            shader.use();
            shader.setMat4("projection", projection);
        }
        static void SetViewMat4(Shader &shader, glm::mat4 view){
            shader.use();
            shader.setMat4("view", view);
        }
        static void SetModelMat4(Shader &shader, glm::mat4 value){
            shader.use();
            shader.setMat4("model", value);
        }
    };
}

#endif //M_BOSSFIGHT_SHADER_EX_H
