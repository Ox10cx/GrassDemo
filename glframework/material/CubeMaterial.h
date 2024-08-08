#ifndef CUBE_MATERIAL_H
#define CUBE_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class CubeMaterial : public Material
{
public:
    CubeMaterial();
    ~CubeMaterial();

public:
    Texture *mDiffuseTexture{nullptr};
};

#endif