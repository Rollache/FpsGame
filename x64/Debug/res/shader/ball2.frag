#version 330 core

in vec3 Normal;
in vec2 TexCoord;
in vec3 Position;
in vec4 PosLightSpace;

out vec4 FragColor;

uniform vec3 color;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // 转换光照空间中的片段坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // 转换为 [0,1] 范围
    float closestDepth = texture(shadowMap, projCoords.xy).r;  // 取出深度值
    float currentDepth = projCoords.z;  // 当前片段深度
    float bias = 0.005;  // 偏移量
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;  // 阴影检测
    return shadow;
}

void main()
{
    vec3 lightColor = vec3(1.0);  // 光源颜色 (白光)
    
    // 环境光
    vec3 ambient = 1.0 * lightColor;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - Position);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // 镜面反射
    vec3 viewDir = normalize(viewPos - Position);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, reflectDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;
    
    // 计算阴影
    float shadow = ShadowCalculation(PosLightSpace);
    
    // 最终颜色（加入阴影效果）
    vec3 result = (ambient + (diffuse + specular)) * color;  // 不再乘以 (1.0 - shadow)，去掉阴影影响
    FragColor = vec4(result, 1.0);  // 输出最终颜色
}
