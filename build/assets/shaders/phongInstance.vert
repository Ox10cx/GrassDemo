#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aUv;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in mat4 aInstanceMatrix;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;
uniform mat3 normalMatrix;

uniform mat4 matrices[100];

void main() {
    //世界坐标系  // 用uniform 实例更新
    // vec4 transformPosition = modelMatrix * matrices[gl_InstanceID] * vec4(aPosition, 1.0);
    
    // 用attribute 实例更新
    vec4 transformPosition = modelMatrix * aInstanceMatrix * vec4(aPosition, 1.0);

    worldPosition = transformPosition.xyz;

    // modelMatrix * aPosition 世界坐标
    // viewMatrix * modelMatrix * aPosition 视图变换，世界坐标转换成摄像机坐标 
    // projectMatrix 正视投影/透视投影矩阵，负责将摄像机坐标转换为NDC坐标
    gl_Position = projectMatrix * viewMatrix * transformPosition;
    uv = aUv;


    // 法线矩阵
    normal = normalMatrix * aNormal;
}