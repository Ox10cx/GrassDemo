#version 330 core

out vec4 fragColor;

in vec3 uvw;

// uniform samplerCube cubeSampler;
uniform sampler2D sphereSampler;


const float PI = 3.1415926535897932384626433832795;


vec2 uvwTouv(vec3 uvwN) {
    // 经线角
    float phi = asin(uvwN.y);
    // 纬线角
    float theta = atan(uvwN.z, uvwN.x);
    float u = theta / (2.0 * PI) + 0.5;
    float v = phi / PI + 0.5;
    return vec2(u, v);
}

void main() {
    // fragColor = texture(cubeSampler, uvw);
    vec3 uvwN = normalize(uvw);
    fragColor = texture(sphereSampler, uvwTouv(uvwN));
}


