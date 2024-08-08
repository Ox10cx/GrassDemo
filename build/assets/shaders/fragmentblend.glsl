#version 330 core

in vec4 color;
in vec2 uv;
out vec4 fragColor;
uniform sampler2D grassSampler;
uniform sampler2D landSampler;
uniform sampler2D noiseSampler;

void main() {
    vec4 grassColor = texture(grassSampler, uv);
    vec4 landColor = texture(landSampler, uv);
    vec4 noiseColor = texture(noiseSampler, uv);
    float weight = noiseColor.r;
    // fragColor = vec4((grassColor * weight + landColor * (1 - weight)).rgb, 1.0);
    // fragColor = vec4(mix(grassColor, landColor, weight).rgb, 1.0);
    fragColor = grassColor;
}