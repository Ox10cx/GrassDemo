#version 330 core

out vec4 fragColor;

uniform float near;
uniform float far;

void main() {

    //  // gl_FragCoord像素坐标位置，颜色混合
    // float flag = step(800, gl_FragCoord.x);
    // vec3 blendColor = mix(vec3(1.0, 0.0, 0.0), vec3(0.0, 0.0, 1.0), flag);

    float zNdc = gl_FragCoord.z * 2.0 - 1;
    float linearDepth = 2.0 * near / (far + near - zNdc * (far - near));

    fragColor = vec4(linearDepth, linearDepth, linearDepth, 1.0) ;
}


