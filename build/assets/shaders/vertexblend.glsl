#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUv;

uniform float time;

out vec4 color;
out vec2 uv;

uniform mat4 transform;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;


void main() {
    // float scale = 1.0 / time;
    // gl_Position = vec4(aPosition.xyz * scale, 1.0);
    // transform * aPosition 世界坐标
    // viewMatrix * transform * aPosition 视图变换，世界坐标转换成摄像机坐标 
    // projectMatrix 正视投影/透视投影矩阵，负责将摄像机坐标转换为NDC坐标
    gl_Position = projectMatrix * viewMatrix * transform * aPosition;
    color = aColor;
    uv = aUv;
    // float detaU = time * 0.3;
    // uv = vec2(aUv.x + detaU, aUv.y);
}