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

struct PointLight {
    vec3  position;
    vec3  color;
    float k2;
    float k1;
    float kc;
    float specularIntensity;
};

struct SpotLight {
    vec3 position;
    vec3 targetDirection;
    vec3 color;
    float innerLine;
    float outerLine;
    float specularIntensity;
};

struct DirectionLight {
    vec3 targetDirection;
    vec3 color;
    float specularIntensity;
};

uniform PointLight pointLight;
uniform SpotLight spotLight;
uniform DirectionLight directionLight;


// 平行光 漫反射
vec3 calDiffuse(vec3 lightColor, vec3 objectColor, vec3 normal, vec3 lightDir) {
    //3 计算最后的颜色  lightColor * diffuse (吸收了多少光照)
    float diffuse = clamp(dot(-lightDir, normal), 0.0, 1.0);

    vec3 diffuseColor = lightColor * diffuse * objectColor;

    return diffuseColor;
}

// 高光反射
vec3 calSpecular(vec3 lightColor, vec3 normal, vec3 lightDir, vec3 viewDir, float specularIntensity) {

    // 计算光的反射向量
    vec3 lightReflect = normalize(reflect(lightDir, normal));

    // 计算 光的反射向量与视线的角度cos
    float specular = clamp(dot(lightReflect, -viewDir), 0.0, 1.0);

    //控制镜面反射光斑大小
    specular = pow(specular, shiness);
    
    // 高光蒙版的效果
    float specularMask = texture(specularMaskSampler, uv).r;

    specularMask = 1.0;

    // 防止背面出现高光的情况
    float dotResult = dot(-lightDir, normal);
    float backResult = step(0.0, dotResult);

    // 计算最终的颜色
    vec3 specularColor = lightColor * specular * backResult * specularIntensity * specularMask;

    return specularColor;
}

// 计算环境光
vec3 calAmbientColor(vec3 objectColor) {
    return ambientLight * objectColor;
}

vec3 calSpotLight(SpotLight spotLight, vec3 objectColor, vec3 normal, vec3 viewDir) {
    // 光源方向
    vec3 lightDir = normalize(worldPosition - spotLight.position);
    // 聚光灯朝向
    vec3 targetDir = normalize(spotLight.targetDirection);

    // 计算探照灯的照射范围
    float cTheta = dot(lightDir, targetDir);

    // // 探照灯可视范围的角度 弧度制
    // float cVisible = cos(visibleAngle);
    // // 判断是否在可视范围内
    // float spotFlag = step(cVisible, cTheta);
   
    // 边缘区域模糊化
    float spotIntensity = clamp((cTheta - spotLight.outerLine) / (spotLight.innerLine - spotLight.outerLine), 0.0, 1.0);

    //漫反射
    vec3 diffuseColor = calDiffuse(spotLight.color, objectColor, normal, lightDir);

    //高光反射
    vec3 specularColor = calSpecular(spotLight.color, normal, lightDir, viewDir, spotLight.specularIntensity);
    

    vec3 fragColor = (diffuseColor + specularColor) * spotIntensity;

    return fragColor;
}

vec3 calDirectionLight(DirectionLight directionLight, vec3 objectColor, vec3 normal, vec3 viewDir) {
    //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 lightDir = normalize(directionLight.targetDirection);

    //漫反射
    vec3 diffuseColor = calDiffuse(directionLight.color, objectColor, normal, lightDir);

    //高光反射
    vec3 specularColor = calSpecular(directionLight.color, normal, lightDir, viewDir, directionLight.specularIntensity);
    
    vec3 fragColor = diffuseColor + specularColor;

    return fragColor;

}

vec3 calPointLight(PointLight pointLight, vec3 objectColor, vec3 normal, vec3 viewDir) {
    // 点光源
    vec3 lightDir = normalize(worldPosition - pointLight.position);
    
    // 计算点光源的衰减系数
    float dist = length(worldPosition - pointLight.position);
    float attenuation = 1.0 / (pointLight.k2 * dist * dist + pointLight.k1 * dist + pointLight.kc);
    
    //漫反射
    vec3 diffuseColor = calDiffuse(pointLight.color, objectColor, normal, lightDir);

    //高光反射
    vec3 specularColor = calSpecular(pointLight.color, normal, lightDir, viewDir, pointLight.specularIntensity);
    
    vec3 fragColor= (diffuseColor + specularColor) * attenuation;

    return fragColor;

}

void main() {
    //1 获取当前物体的像素颜色
    vec3 objectColor = texture(sampler, uv).xyz;

    //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 normalN = normalize(normal);
 
    // 计算视线方向
    vec3 viewDir = normalize(worldPosition - cameraPosition);

    // 计算探照灯的颜色
    vec3 spotLightColor = calSpotLight(spotLight, objectColor, normalN, viewDir);

    // 计算平行光的颜色
    vec3 directionLightColor = calDirectionLight(directionLight, objectColor, normalN, viewDir);

    // 计算点光源颜色
    vec3 pointLightColor = calPointLight(pointLight, objectColor, normalN, viewDir);

    //环境光的计算
    vec3 ambientColor = calAmbientColor(objectColor);


    fragColor = vec4(spotLightColor + directionLightColor + pointLightColor + ambientColor, 1.0) ;
}



