#version 330 core

in vec3 ourColor; // 从顶点着色器传递的颜色
out vec4 FragColor; // 最终输出的颜色

void main() {
    FragColor = vec4(ourColor, 1.0); // 设置片段颜色为传递的颜色，透明度为1（不透明）
}
