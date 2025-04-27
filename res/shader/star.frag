#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D diffuse;

void main()
{
    vec3 color = texture(diffuse, TexCoord).rgb;
    FragColor = vec4(color, 1.0);
}
