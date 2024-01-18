#version 330 core
#define MAX_BONE_INFLUENCE = 4;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
//layout (location = 4) in int aBoneIDs[MAX_BONE_INFLUENCE];
//layout (location = 5) in float aWeights[MAX_BONE_INFLUENCE];
layout (location = 4) in float aUseTextures;
layout (location = 5) in vec3  color_diffuse;
layout (location = 6) in vec3  color_specular;
layout (location = 7) in vec3  color_normal;
layout (location = 8) in vec3  color_height;
layout (location = 9) in vec3  color_emission;
layout (location = 10) in vec3  color_shininess;
layout (location = 11) in float aShininess;

out VS_OUT
{
    float UseTexture;
    vec3 Normal;
    vec3 FragPos;
    mat3 TBN;
} vs_out;

out MaterialColor {
    vec3  color_diffuse;
    vec3  color_specular;
    vec3  color_normal;
    vec3  color_height;
    vec3  color_shininess;
    vec3  color_emission;
    float useEmission;
    float shininess;
} materialColor;


uniform mat4 model;
uniform mat3 normalModel;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.Normal = normalModel * aNormal;
    vs_out.UseTexture = aUseTextures;
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

    //Gram-Schmidt process
    vec3 T = normalize(vec3(model * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);
    vs_out.TBN = mat3(T, B, N);

    materialColor.color_diffuse = color_diffuse;
    materialColor.color_specular = color_specular;
    materialColor.color_height = color_height;
    materialColor.color_normal = color_normal;
    materialColor.color_emission = color_emission;
    materialColor.color_shininess = color_shininess;
    materialColor.shininess = aShininess;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}