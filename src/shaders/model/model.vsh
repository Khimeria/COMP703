#version 330 core
#define MAX_BONE_INFLUENCE = 4;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
//layout (location = 5) in int aBoneIDs[MAX_BONE_INFLUENCE];
//layout (location = 6) in float aWeights[MAX_BONE_INFLUENCE];
layout (location = 7) in vec4 aColor;
layout (location = 8) in float aUseTexture;

out VS_OUT
{
    vec2 TexCoords;
    vec4 Color;
    float UseTexture;
    vec3 Normal;
    vec3 FragPos;
    mat3 TBN;
} vs_out;


uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.TexCoords = aTexCoords;
    vs_out.Color = aColor;
    vs_out.Normal = normalModel * aNormal;
    vs_out.UseTexture = aUseTexture;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

    //Gram-Schmidt process
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}