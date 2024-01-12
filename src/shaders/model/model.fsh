#version 330 core
struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
};
struct Light {
        vec3 position;

        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
};

out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;
in float UseTexture;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform Light light;

uniform sampler2D texture_diffuse1;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
        // ambient
        vec3 ambient  = lightColor * light.ambient * material.ambient;

        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse  = lightColor * light.diffuse * (diff * material.diffuse);

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = lightColor * light.specular * (spec * material.specular);

        vec3 result = ambient + diffuse + specular;

        if(UseTexture == 1.0)
        {
                FragColor = texture(texture_diffuse1, TexCoords)*vec4(result,1);
        }
        else
        {
                FragColor = Color*vec4(result,1);
        }

}