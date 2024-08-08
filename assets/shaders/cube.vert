#version 330 core

layout(location = 0) in vec3 aPosition;

out vec3 uvw;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectMatrix;


void main() {
    //世界坐标系
    vec4 transformPosition = modelMatrix * vec4(aPosition, 1.0);

    // modelMatrix * aPosition 世界坐标
    // viewMatrix * modelMatrix * aPosition 视图变换，世界坐标转换成摄像机坐标 
    // projectMatrix 正视投影/透视投影矩阵，负责将摄像机坐标转换为NDC坐标
    gl_Position = projectMatrix * viewMatrix * transformPosition;

    gl_Position = gl_Position.xyww;
    uvw = aPosition;
}