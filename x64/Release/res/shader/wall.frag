 #version 330 core
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;   
uniform vec4 uColor;       
uniform bool useColor;       

void main()
{
    vec4 texColor = texture(uTexture, TexCoord);

    if (useColor)
    {
        FragColor = vec4(mix(texColor.rgb, uColor.rgb, uColor.a), texColor.a);
    }
    else
    {
        FragColor = texColor;
    }
}
