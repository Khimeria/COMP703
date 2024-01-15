#version 330 core

#define NR_TEX_NUM 1
struct Material {
        sampler2D texture_diffuse[NR_TEX_NUM];
        sampler2D texture_specular[NR_TEX_NUM];
        sampler2D texture_normal[NR_TEX_NUM];
        sampler2D texture_height[NR_TEX_NUM];
        sampler2D texture_emission[NR_TEX_NUM];
        vec3      emissionColor;
        float     useEmission;
        float     shininess;
};
uniform Material material;

struct DirectLight {
        vec3 direction;
        vec3 color;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};
uniform DirectLight directLight;

struct PointLight {
        vec3 position;
        vec3 color;

        float constant;
        float linear;
        float quadratic;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
        vec3 direction;
        vec3 position;
        vec3 color;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        float constant;
        float linear;
        float quadratic;

        float cutOff;

        float outerCutOff;
};
#define NR_SPOT_LIGHTS 1
uniform SpotLight spotLights[NR_SPOT_LIGHTS];
uniform vec3 viewPos;

in VS_OUT
{
        vec2 TexCoords;
        vec4 Color;
        float UseTexture;
        vec3 Normal;
        vec3 FragPos;
        mat3 TBN;
} fs_in;

out vec4 FragColor;

//functions
vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float CalcAttenuation(vec3 fragToLightDir, float constant, float linear, float quadratic);

vec3 CalcAmbient(vec3 lightAmbient, vec3 normLightDir, vec3 normal, vec3 viewDir);
vec3 CalcDiffuse(vec3 lightDiffuse, vec3 normLightDir, vec3 normal, vec3 viewDir);
vec3 CalcSpecular(vec3 lightSpecular, vec3 normLightDir, vec3 normal, vec3 viewDir);
vec3 CalcEmission();

void main()
{

        // properties
        vec3 normal = texture(material.texture_normal[0], fs_in.TexCoords).rgb;
        // transform normal vector to range [-1,1]
        vec3 norm = normalize(normal * 2.0 - 1.0);
        norm = normalize(fs_in.TBN * normal);

        //vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);

        // phase 1: Directional lighting
        vec3 result = CalcDirectLight(directLight, norm, viewDir);
        // phase 2: Point lights
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
                result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);
        // phase 3: Spot light
        for(int i = 0; i < NR_SPOT_LIGHTS; i++)
                result += CalcSpotLight(spotLights[i], norm, fs_in.FragPos, viewDir);

        FragColor = vec4(result, 1.0);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
        vec3 lightDir = normalize(-light.direction);

        vec3 ambient  = CalcAmbient(light.ambient, lightDir, normal, viewDir);
        vec3 diffuse  = CalcDiffuse(light.diffuse, lightDir, normal, viewDir);
        vec3 specular = CalcSpecular(light.specular, lightDir, normal, viewDir);
        vec3 emission = CalcEmission();

        vec3 result = light.color *(ambient + diffuse + specular) + emission;
        return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
        vec3 fragToLightDir = light.position - fragPos;
        float attenuation = CalcAttenuation(fragToLightDir, light.constant, light.linear, light.quadratic);

        vec3 lightDir = normalize(fragToLightDir);

        vec3 ambient  = CalcAmbient(light.ambient, lightDir, normal, viewDir);
        vec3 diffuse  = CalcDiffuse(light.diffuse, lightDir, normal, viewDir);
        vec3 specular = CalcSpecular(light.specular, lightDir, normal, viewDir);
        vec3 emission = CalcEmission();

        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        return light.color * (ambient + diffuse + specular) + emission;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
        vec3 fragToLightDir = light.position - fragPos;
        vec3 lightDir = normalize(fragToLightDir);
        float theta     = dot(lightDir, normalize(-light.direction));

        float attenuation = CalcAttenuation(fragToLightDir, light.constant, light.linear, light.quadratic);

        if(theta > light.outerCutOff)
        {
                //intensity
                float epsilon   = light.cutOff - light.outerCutOff;
                float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

                vec3 ambient  = CalcAmbient(light.ambient, lightDir, normal, viewDir);
                vec3 diffuse  = CalcDiffuse(light.diffuse, lightDir, normal, viewDir);
                vec3 specular = CalcSpecular(light.specular, lightDir, normal, viewDir);
                vec3 emission = CalcEmission();

                diffuse  *= intensity;
                specular *= intensity;

                vec3 result = light.color * attenuation * (ambient + diffuse + specular);
                result += emission;
                return result;
        }
        return attenuation * light.color *  light.ambient * texture(material.texture_diffuse[0], fs_in.TexCoords).rgb;
}

vec3 CalcAmbient(vec3 lightAmbient, vec3 normLightDir, vec3 normal, vec3 viewDir)
{
        return lightAmbient  * texture(material.texture_height[0], fs_in.TexCoords).rgb;
}

vec3 CalcDiffuse(vec3 lightDiffuse, vec3 normLightDir, vec3 normal, vec3 viewDir)
{
        // diffuse shading
        float diff = max(dot(normal, normLightDir), 0.0);
        return lightDiffuse  * diff * texture(material.texture_diffuse[0], fs_in.TexCoords).rgb;

}

vec3 CalcSpecular(vec3 lightSpecular, vec3 normLightDir, vec3 normal, vec3 viewDir)
{
        vec3 halfwayDir = normalize(normLightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

        return lightSpecular * spec * texture(material.texture_specular[0], fs_in.TexCoords).rgb;

}

float CalcAttenuation(vec3 fragToLightDir, float constant, float linear, float quadratic)
{
        float distance = length(fragToLightDir);
        return 1.0 / (constant + linear * distance + quadratic * pow(distance, 2.0));
}

vec3 CalcEmission()
{
        vec3 emission = vec3(0.0);
        if(material.useEmission == 1.0){
                emission = texture(material.texture_emission[0], fs_in.TexCoords).rgb;
        }
        return emission;
}