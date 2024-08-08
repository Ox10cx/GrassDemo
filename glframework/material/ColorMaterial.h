#ifndef COLOR_MATERIAL_H
#define COLOR_MATERIAL_H

#include "Material.h"

class ColorMaterial : public Material
{
public:
    ColorMaterial();
    ~ColorMaterial();

    void setKa(const glm::vec3& ka) {this->mKa = ka;}
    void setKd(const glm::vec3& kd) {this->mKd = kd;}
    void setKs(const glm::vec3& ks) {this->mKs = ks;}
    void setShiness(float shiness) {this->mShiness = shiness;}

public:
   // 材质的环境光颜色
   glm::vec3 mKa{0.0f};
   // 材质的漫反射颜色
   glm::vec3 mKd{0.0f};
   // 材质的镜面反射颜色
   glm::vec3 mKs{0.0f};
   float mShiness{1.0f};
};

#endif