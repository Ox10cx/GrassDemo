#ifndef OPACITY_MATERIAL_H
#define OPACITY_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class OpacityMaterial : public Material
{
public:
    OpacityMaterial();
    ~OpacityMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mOpacityMask{nullptr};
    float mShiness{1.0f};
};

#endif