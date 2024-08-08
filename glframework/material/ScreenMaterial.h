#ifndef SCREEN_MATERIAL_H
#define SCREEN_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class ScreenMaterial : public Material
{
public:
    ScreenMaterial();
    ~ScreenMaterial();

public:
    Texture *mScreenTexture{nullptr};
};

#endif