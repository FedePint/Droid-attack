#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse3;
uniform sampler2D texture_specular3;

void main()
{    
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Rosso pieno
}