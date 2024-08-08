#ifndef PHONG_INSTANCE_MATERIAL_H
#define PHONG_INSTANCE_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class PhongInstanceMaterial : public Material
{
public:
    PhongInstanceMaterial();
    ~PhongInstanceMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mSpecularMask{nullptr};
    float mShiness{1.0f};
};

#endif