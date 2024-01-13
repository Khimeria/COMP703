#version 330 core

struct Material {
        sampler2D texture_diffuse1;
        sampler2D texture_specular1;
        sampler2D texture_normal1;
        sampler2D texture_height1;
        sampler2D texture_emmision1;
        float     shininess;
};

struct MaterialOld {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
};

struct Light {
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

out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;
in float UseTexture;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform MaterialOld material2;
uniform Light light;

uniform vec3 viewPos;

void main()
{

        //vec3 lightDir = normalize(-light.direction);
        vec3 lightDir = normalize(light.position - FragPos);
        float theta     = dot(lightDir, normalize(-light.direction));

        //attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

        if(theta > light.outerCutOff)
        {
                //intensity
                float epsilon   = light.cutOff - light.outerCutOff;
                float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

                // diffuse
                vec3 norm = normalize(Normal);
                float diff = max(dot(norm, lightDir), 0.0);
                // specular
                vec3 viewDir = normalize(viewPos - FragPos);
                vec3 reflectDir = reflect(-lightDir, norm);
                float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

                if(UseTexture == 1.0)
                {
                        vec3 ambient  = light.ambient  * texture(material.texture_diffuse1, TexCoords).rgb;
                        vec3 diffuse  = light.diffuse  * diff * texture(material.texture_diffuse1, TexCoords).rgb;
                        vec3 specular = light.specular * spec * texture(material.texture_specular1, TexCoords).rgb;
                        //vec3 emission = texture(material.texture_emmision1, TexCoords).rgb;
                        diffuse  *= intensity;
                        specular *= intensity;

                        vec3 result = attenuation * (ambient + diffuse + specular);
                        //result += emission;
                        FragColor = vec4(light.color * result, 1.0);
                }
                else
                {
                        vec3 ambient  = light.color * light.ambient * material2.ambient;
                        vec3 diffuse  = light.color * light.diffuse * (diff * material2.diffuse);
                        vec3 specular = light.color * light.specular * (spec * material2.specular);
                        diffuse  *= intensity;
                        specular *= intensity;
                        vec4 result = vec4(attenuation * (ambient + diffuse + specular),1);
                        FragColor = Color*result;
                }
        }
        else  // else, use ambient light so scene isn't completely dark outside the spotlight.
                FragColor = vec4(attenuation * light.color * light.ambient * texture(material.texture_diffuse1, TexCoords).rgb, 1.0);



}