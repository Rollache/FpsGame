#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D screenTexture; 
uniform float redIntensity;

void main()
{
    vec4 originalColor = texture(screenTexture, TexCoord); 
    vec3 redTint = vec3(1.0, 0.0, 0.0); 

    vec3 finalColor = mix(originalColor.rgb, redTint, redIntensity);

    FragColor = vec4(finalColor, originalColor.a);
}
