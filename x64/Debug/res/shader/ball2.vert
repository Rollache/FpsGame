#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normal;
out vec2 TexCoord;
out vec3 Position;
out vec4 PosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main()
{
    // 计算当前顶点的世界空间位置
    Position = vec3(model * vec4(aPosition, 1.0));
    // 计算法向量
    Normal = transpose(inverse(mat3(model))) * aNormal;
    // 传递纹理坐标
    TexCoord = aTexCoord;
    // 计算光照空间中的顶点位置
    PosLightSpace = lightSpaceMatrix * vec4(Position, 1.0);
    // 计算最终位置
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
