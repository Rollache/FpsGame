#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1; // 绑定的纹理
uniform float alpha; // 透明度控制

void main()
{
    vec4 texColor = texture(texture1, TexCoord); // 读取贴图颜色
    FragColor = vec4(texColor.rgb, texColor.a * alpha);  // 透明度乘以 alpha
}
