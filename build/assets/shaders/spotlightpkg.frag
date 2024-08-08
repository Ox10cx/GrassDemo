#version 330 core

in vec2 uv;
in vec3 normal;
in vec3 worldPosition;
out vec4 fragColor;

uniform sampler2D sampler;
// 高光蒙版贴图
uniform sampler2D specularMaskSampler;

uniform vec3 ambientLight;
uniform vec3 cameraPosition;
uniform float shiness;

struct SpotLight {
    vec3 position;
    vec3 targetDirection;
    vec3 color;
    float innerLine;
    float outerLine;
    float specularIntensity;

};

uniform SpotLight spotLight;

// 平行光 漫反射
vec3 calDiffuse(vec3 objectColor, vec3 normalN, vec3 lightDirN) {
    //3 计算最后的颜色  lightColor * diffuse (吸收了多少光照)
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0, 1.0);

    vec3 diffuseColor = spotLight.color * diffuse * objectColor;

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
    vec3 specularColor = spotLight.color * specular * backResult * spotLight.specularIntensity * specularMask;

    return specularColor;
}

// 计算环境光
vec3 calAmbientColor(vec3 objectColor) {
    
    return ambientLight * objectColor;
}

float calSpotIntensity(vec3 lightDirN, vec3 targetDirN)
{
    // 计算探照灯的照射范围
    float cTheta = dot(lightDirN, targetDirN);

    // // 探照灯可视范围的角度 弧度制
    // float cVisible = cos(visibleAngle);
    // // 判断是否在可视范围内
    // float spotFlag = step(cVisible, cTheta);
   
    // 边缘区域模糊化
    float intensity =clamp((cTheta - spotLight.outerLine) / (spotLight.innerLine - spotLight.outerLine), 0.0, 1.0);

    return intensity;
}

void main() {
    //1 获取当前物体的像素颜色
    vec3 objectColor = texture(sampler, uv).xyz;

    //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 normalN = normalize(normal);

    // 光源方向
    vec3 lightDirN = normalize(worldPosition - spotLight.position);
 
    // 聚光灯朝向
    vec3 targetDirN = normalize(spotLight.targetDirection);

    // 计算视线方向
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    // 计算探照灯的区域
    float spotIntensity = calSpotIntensity(lightDirN, targetDirN);

    //漫反射
    vec3 diffuseColor = calDiffuse(objectColor, normalN, lightDirN);

    //高光反射
    vec3 specularColor = calSpecular(normalN, lightDirN, viewDir);

    //环境光的计算
    vec3 ambientColor = calAmbientColor(objectColor);

    fragColor = vec4((diffuseColor + specularColor) * spotIntensity + ambientColor, 1.0) ;
}


