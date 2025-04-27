#version 330 core
in vec3 ourColor; // 从顶点着色器接收的颜色
out vec4 FragColor;

void main() {
    FragColor = vec4(ourColor, 1.0); // 使用顶点颜色绘制
}
