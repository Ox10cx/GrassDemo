#version 330 core

in vec4 color;
in vec2 uv;
out vec4 fragColor;
// uniform float time;
uniform sampler2D sampler;

void main() {
    // fragColor = color * ((sin(time) + 1.0) / 2.0);
    fragColor = texture(sampler, uv);
}