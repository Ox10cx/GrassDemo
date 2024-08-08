#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
in vec2 worldPosXZ;
out vec4 fragColor;

uniform sampler2D sampler;
uniform sampler2D opacityMaskSampler;

// 云层
uniform sampler2D cloudMaskSampler;
uniform vec3 cloudWhiteColor;
uniform vec3 cloudBlackColor;
uniform float cloudUvScale;
uniform float cloudSpeed;
uniform float cloudLerp;


// // 高光蒙版贴图
// uniform sampler2D specularMaskSampler;

uniform float time;
uniform vec3 windDirection;

// 草地贴图的特性
uniform float uvScale;
// 草地贴图的亮度
uniform float brightness;

uniform float opacity;

uniform vec3 lightDirection;
uniform vec3 lightColor;

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
    
    // // 高光蒙版的效果
    // float specularMask = texture(specularMaskSampler, uv).r;

    float specularMask = 1.0f;

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
    vec2 worldPosUV = worldPosXZ / uvScale;

    //1 获取当前物体的像素颜色
    vec3 objectColor = texture(sampler, worldPosUV).xyz * brightness;

    //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(lightDirection);

    // 计算视线方向
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    //漫反射
    vec3 diffuseColor = calDiffuse(objectColor, normalN, lightDirN);

    //高光反射
    vec3 specularColor = calSpecular(normalN, lightDirN, viewDir);

    //环境光的计算
    vec3 ambientColor = calAmbientColor(objectColor);

    // 草地颜色
    vec3 grassColor = diffuseColor + specularColor + ambientColor;

    // 透明模版的a
    float alpha = texture(opacityMaskSampler, uv).r;

    // 云层模版
    vec3 windDirN = normalize(windDirection);
    vec2 cloudPosUV = worldPosXZ / cloudUvScale;
    cloudPosUV = cloudPosUV + windDirN.xz * time * cloudSpeed;
    float cloudMask = texture(cloudMaskSampler, cloudPosUV).r;
    // 云层颜色
    vec3 cloudColor = mix(cloudBlackColor, cloudWhiteColor, cloudMask);

    // 草地和云层颜色混合
    vec3 finalColor = mix(grassColor, cloudColor, cloudLerp);

    fragColor = vec4(finalColor, alpha * opacity);
    
}



// // 平行光 漫反射
// vec4 calDiffuse() {

//     //1 获取当前物体的像素颜色
//     vec3 objectColor = texture(sampler, uv).xyz;

//     //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
//     vec3 normalN = normalize(normal);
//     vec3 lightDirN = normalize(lightDirection);

//     //3 计算最后的颜色  lightColor * diffuse (吸收了多少光照)
//     float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);
//     vec4 diffuseColor = vec4(lightColor * diffuse * objectColor, 1.0);

//     return diffuseColor;
// }
