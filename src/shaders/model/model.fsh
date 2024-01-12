#version 330 core
out vec4 FragColor;

in vec4 Color;
in vec2 TexCoords;
in float UseTexture;

uniform vec3 lightColor;

uniform sampler2D texture_diffuse1;

void main()
{
        if(UseTexture == 1.0f)
        {
                FragColor = texture(texture_diffuse1, TexCoords)*vec4(lightColor,1);
        }
        else
        {
                FragColor = Color*vec4(lightColor,1);
        }

}