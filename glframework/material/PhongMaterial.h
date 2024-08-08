#ifndef PHONG_MATERIAL_H
#define PHONG_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class PhongMaterial : public Material
{
public:
    PhongMaterial();
    ~PhongMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mSpecularMask{nullptr};
    float mShiness{1.0f};
};

#endif