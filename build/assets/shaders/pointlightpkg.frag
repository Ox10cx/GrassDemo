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

uniform PointLight pointLight;

// 平行光 漫反射
vec3 calDiffuse(vec3 objectColor,vec3 normalN,vec3 lightDirN){
    //3 计算最后的颜色  lightColor * diffuse (吸收了多少光照)
    float diffuse=clamp(dot(-lightDirN,normalN),0.,1.);
    
    vec3 diffuseColor=pointLight.color*diffuse*objectColor;
    
    return diffuseColor;
}

// 高光反射
vec3 calSpecular(vec3 normalN,vec3 lightDirN,vec3 viewDir){
    // 计算光的反射向量
    vec3 lightReflect=normalize(reflect(lightDirN,normalN));
    
    // 计算 光的反射向量与视线的角度cos
    float specular=clamp(dot(lightReflect,-viewDir),0.,1.);
    
    //控制镜面反射光斑大小
    specular=pow(specular,shiness);
    
    // 高光蒙版的效果
    float specularMask=texture(specularMaskSampler,uv).r;
    
    // 防止背面出现高光的情况
    float dotResult=dot(-lightDirN,normalN);
    float backResult=step(0.,dotResult);
    
    // 计算最终的颜色
    vec3 specularColor= pointLight.color * specular * backResult * pointLight.specularIntensity * specularMask;
    
    return specularColor;
}

// 计算环境光
vec3 calAmbientColor(vec3 objectColor){
    
    return ambientLight*objectColor;
}

void main(){
    //1 获取当前物体的像素颜色
    vec3 objectColor=texture(sampler,uv).xyz;
    
    //2 准备diffuse漫反射 各类相关的数据 （法线/光的方向）
    vec3 normalN=normalize(normal);
    
    // 点光源
    vec3 lightDirN=normalize(worldPosition - pointLight.position);
    
    // 计算视线方向
    vec3 viewDir=normalize(worldPosition - cameraPosition);
    
    // 计算点光源的衰减系数
    float dist=length(worldPosition - pointLight.position);
    float attenuation = 1.0 /(pointLight.k2 * dist * dist + pointLight.k1 * dist + pointLight.kc);
    
    //漫反射
    vec3 diffuseColor=calDiffuse(objectColor,normalN,lightDirN);
    
    //高光反射
    vec3 specularColor=calSpecular(normalN,lightDirN,viewDir);
    
    //环境光的计算
    vec3 ambientColor=calAmbientColor(objectColor);
    
    fragColor=vec4((diffuseColor+specularColor)*attenuation+ambientColor,1.);
}

