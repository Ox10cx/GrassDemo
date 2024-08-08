#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
out vec4 fragColor;

uniform sampler2D sampler;
// 高光蒙版贴图
uniform sampler2D specularMaskSampler;


uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float k2;
uniform float k1;
uniform float kc;

uniform vec3 ambientLight;

uniform vec3 cameraPosition;
uniform float specularIntensity;
uniform float shiness;

// 平行光 漫反射
vec3 calDiffuse(vec3 objectColor, vec3 normalN, vec3 lightDirN) {
    //3 计算最后的颜色  lightColor * diffuse (吸收了多少光照)
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);

    vec3 diffuseColor = lightColor * diffuse * objectColor;

    return diffuseColor;
}

// 高光反射
vec3 calSpecular(vec3 normalN, vec3 lightDirN, vec3 viewDir) {
    // 计算光的反射向量
    vec3 lightReflect = normalize(reflect(lightDirN, normalN));

    // 计算 光的反射向量与视线的角度cos
    float specular = clamp(dot(lightReflect, -viewDir), 0.0, 1.0);

    //控制镜面反射光斑大小
    specular = pow(specular, shiness);
    
    // 高光蒙版的效果
    float specularMask = texture(specularMaskSampler, uv).r;

    // 防止背面出现高光的情况
    float dotResult = dot(-lightDirN, normalN);
    float backResult = step(0.0, dotResult);

    // 计算最终的颜色
    vec3 specularColor = lightColor * specular * backResult * specularIntensity * specularMask;

    return specularColor;
}

// 计算环境光
vec3 calAmbientColor(vec3 objectColor) {
    
    return ambientLight * objectColor;
}

void main() {
    //1 获取当前物体的像素颜色
    vec3 objectColor = texture(sampler, uv).xyz;

    //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 normalN = normalize(normal);

    // 点光源
    vec3 lightDirN = normalize(worldPosition - lightPosition);

    // 计算视线方向
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    // 计算点光源的衰减系数
    float dist = length(worldPosition - lightPosition);
    float attenuation = 1.0 / (k2 * dist * dist + k1 * dist + kc);

    //漫反射
    vec3 diffuseColor = calDiffuse(objectColor, normalN, lightDirN);

    //高光反射
    vec3 specularColor = calSpecular(normalN, lightDirN, viewDir);

    //环境光的计算
    vec3 ambientColor = calAmbientColor(objectColor);

    fragColor = vec4((diffuseColor + specularColor) * attenuation + ambientColor, 1.0) ;
}


