#version 330 core

layout(location = 0) in vec4 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aUv;

uniform float time;

out vec4 color;
out vec2 uv;


void main() {
    gl_Position = aPosition;
    color = aColor;
    float detaU = time * 0.3;
    uv = vec2(aUv.x + detaU, aUv.y);
}