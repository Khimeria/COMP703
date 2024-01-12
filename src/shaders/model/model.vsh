#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 7) in vec4 aColor;
layout (location = 8) in float aUseTexture;

out vec2 TexCoords;
out vec4 Color;
out float UseTexture;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoords = aTexCoords;
    Color = aColor;
    Normal = normalModel * aNormal;
    UseTexture = aUseTexture;

    FragPos = vec3(model * vec4(aPos, 1.0));

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}