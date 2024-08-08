#ifndef CUBE_ENV_MATERIAL_H
#define CUBE_ENV_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class CubeEnvMaterial : public Material
{
public:
    CubeEnvMaterial();
    ~CubeEnvMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mSpecularMask{nullptr};
    Texture *mEnv{nullptr};
    float mShiness{1.0f};
};

#endif