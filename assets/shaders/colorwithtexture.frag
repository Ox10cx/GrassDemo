#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
out vec4 fragColor;

uniform bool hasDiffuseTexure;
uniform sampler2D diffuseSampler;

uniform float opacity;

// 高光蒙版贴图
uniform bool hasSpecularMaskTexure;
uniform sampler2D specularMaskSampler;


uniform vec3 lightDirection;
uniform vec3 lightColor;

uniform vec3 ambientLight;

uniform vec3 cameraPosition;
uniform float specularIntensity;
uniform float shiness;

struct MatColor {
    vec3 aAmbient;
    vec3 aDiffuse;
    vec3 aSpecular;
};

uniform MatColor matColor;

// 平行光 漫反射
vec3 calDiffuse(vec3 normalN, vec3 lightDirN) {
    //1 当前物体的漫反射像素颜色
    vec3 matDiffuseColor = vec3(1.0, 1.0, 1.0);
    if (hasDiffuseTexure) {
        matDiffuseColor = texture(diffuseSampler, uv).xyz;
    } else {
        matDiffuseColor = matColor.aDiffuse;
    }

    //3 计算最后的颜色  lightColor * diffuse (吸收了多少光照)
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);

    vec3 diffuseColor = lightColor * diffuse * matDiffuseColor;

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
    float specularMask  = 1.0;
    if (hasSpecularMaskTexure) {
        specularMask = texture(specularMaskSampler, uv).r;
    }

    // 防止背面出现高光的情况
    float dotResult = dot(-lightDirN, normalN);
    float backResult = step(0.0, dotResult);

    // 计算最终的颜色
    vec3 specularColor = lightColor * specular * backResult * specularIntensity * specularMask * matColor.aSpecular;

    return specularColor;
}

// 计算环境光
vec3 calAmbientColor() {
    
    return ambientLight * matColor.aAmbient;
}

void main() {

    // 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(lightDirection);

    // 计算视线方向
    vec3 viewDir = normalize(worldPosition - cameraPosition);


    //漫反射
    vec3 diffuseColor = calDiffuse(normalN, lightDirN);

    //高光反射
    vec3 specularColor = calSpecular(normalN, lightDirN, viewDir);

    //环境光的计算
    vec3 ambientColor = calAmbientColor();
    
    float alpha = 1.0;
    if (hasDiffuseTexure) {
        alpha = texture(diffuseSampler, uv).a;
    }
    alpha = alpha * opacity;


    fragColor = vec4(diffuseColor + specularColor + ambientColor, alpha) ;
}


// void main() {

//     // 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
//     vec3 normalN = normalize(normal);
//     vec3 lightDirN = normalize(lightDirection);

//     // 计算视线方向
//     vec3 viewDir = normalize(worldPosition - cameraPosition);


//     //漫反射
//     vec3 diffuseColor = calDiffuse(normalN, lightDirN);

//     //高光反射
//     vec3 specularColor = calSpecular(normalN, lightDirN, viewDir);

//     //环境光的计算
//     vec3 ambientColor = calAmbientColor();

//     fragColor = vec4(diffuseColor + specularColor + ambientColor, 1.0) ;
// }


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
