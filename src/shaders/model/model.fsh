#version 330 core

#define NR_TEX_NUM 1
struct Material {
        sampler2D texture_diffuse[NR_TEX_NUM];
        sampler2D texture_specular[NR_TEX_NUM];
        sampler2D texture_normal[NR_TEX_NUM];
        sampler2D texture_height[NR_TEX_NUM];
        sampler2D texture_emission[NR_TEX_NUM];
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

in vec4 Color;
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in float UseEmissiveTexture;

out vec4 FragColor;

//functions
vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
        // properties
        vec3 normal = texture(material.texture_normal[0], TexCoords).rgb;
        // transform normal vector to range [-1,1]
        vec3 norm = normalize(normal * 2.0 - 1.0);
        //vec3 norm = normalize(Normal);
        vec3 viewDir = normalize(viewPos - FragPos);

        // phase 1: Directional lighting
        vec3 result = CalcDirectLight(directLight, norm, viewDir);
        // phase 2: Point lights
        for(int i = 0; i < NR_POINT_LIGHTS; i++)
                result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
        // phase 3: Spot light
        for(int i = 0; i < NR_SPOT_LIGHTS; i++)
                result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

        FragColor = vec4(result, 1.0);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir)
{
        vec3 lightDir = normalize(-light.direction);
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        vec3 ambient  = light.ambient  * texture(material.texture_diffuse[0], TexCoords).rgb;
        vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse[0], TexCoords).rgb;
        vec3 specular = light.specular * spec * texture(material.texture_specular[0], TexCoords).rgb;
        vec3 emission = vec3(0.0);
        if(material.useEmission == 1.0){
                 texture(material.texture_emission[0], TexCoords).rgb;
        }


        vec3 result = light.color *(ambient + diffuse + specular + emission);
        return result;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
        vec3 lightDir = normalize(light.position - fragPos);
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance +
        light.quadratic * (distance * distance));
        // combine results
        vec3 ambient  = light.ambient  * texture(material.texture_diffuse[0], TexCoords).rgb;
        vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse[0], TexCoords).rgb;
        vec3 specular = light.specular * spec * texture(material.texture_specular[0], TexCoords).rgb;

        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        return light.color * (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal,vec3 fragPos, vec3 viewDir)
{
        vec3 lightDir = light.position - fragPos;
        vec3 n_lightDir = normalize(lightDir);
        float theta     = dot(n_lightDir, normalize(-light.direction));

        //attenuation
        float distance    = length(lightDir);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

        if(theta > light.outerCutOff)
        {
                //intensity
                float epsilon   = light.cutOff - light.outerCutOff;
                float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

                // diffuse
                float diff = max(dot(normal, n_lightDir), 0.0);
                // specular
                vec3 viewDir = normalize(viewPos - fragPos);
                vec3 reflectDir = reflect(-n_lightDir, normal);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

                vec3 ambient  = light.ambient  * texture(material.texture_diffuse[0], TexCoords).rgb;
                vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse[0], TexCoords).rgb;
                vec3 specular = light.specular * spec * texture(material.texture_specular[0], TexCoords).rgb;
                //vec3 emission = texture(material.texture_emmision1, TexCoords).rgb;
                diffuse  *= intensity;
                specular *= intensity;

                vec3 result = light.color * attenuation * (ambient + diffuse + specular);
                //result += emission;
                return result;
        }
        return attenuation * light.color *  light.ambient * texture(material.texture_diffuse[0], TexCoords).rgb;
}