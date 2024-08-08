#version 330 core

out vec4 fragColor;
in vec2 uv;

uniform sampler2D screenTexSampler;
uniform int texWidth;
uniform int texHeight;

vec3 blur() {
    float du = 1.0 / texWidth;
    float dv = 1.0 / texHeight;

    vec2 offsets[9] = vec2[](
        vec2(-du, dv),  //左上
        vec2(0.0, dv),  //正上
        vec2( du, dv),  //右上
        vec2(-du, 0.0),   // 左
        vec2(0.0, 0.0),   // 中
        vec2( du, 0.0),   // 右
        vec2(-du, -dv), // 左下
        vec2(0.0, -dv), // 正下
        vec2( du, -dv)  // 右下
        );

    // 卷积核
    float kernel[9] = float[](
        1.0, 2.0, 1.0,
        2.0, 4.0, 2.0,
        1.0, 2.0, 1.0
    );

    // 加权相加
    vec3 sumColor = vec3(0.0);
    for (int i = 0; i < 9; i++) {
        vec3 samplerColor = texture(screenTexSampler, uv + offsets[i]).rgb;
        sumColor += samplerColor * kernel[i];
    }
    sumColor /= 16.0;

    return sumColor;

}


vec3 revertColor(vec3 color) {
    return vec3(1.0) - color;
}

vec3 gray(vec3 color) {
    // float avg = (color.r + color.g, color.b) / 3.0;
    // 人眼对绿色比较敏感
    float avg = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
    return vec3(avg);
}

void main() {
    vec4 objColor = texture(screenTexSampler, uv);
    // fragColor = vec4(revertColor(objColor.rgb), 1.0);
    // fragColor = vec4(gray(objColor.rgb), 1.0);

    fragColor = vec4(blur(), 1.0);
}


