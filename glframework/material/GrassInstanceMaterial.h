#ifndef GRASS_INSTANCE_MATERIAL_H
#define GRASS_INSTANCE_MATERIAL_H

#include "Material.h"
#include "../Texture.h"

class GrassInstanceMaterial : public Material
{
public:
    GrassInstanceMaterial();
    ~GrassInstanceMaterial();

public:
    Texture *mDiffuse{nullptr};
    Texture *mSpecularMask{nullptr};
    Texture *mOpacityMask{nullptr};
    float mShiness{1.0f};

    // 草地贴图特性
    float mUvScale {1.0f};
    float mBrightness {1.0f};

    // 风力相关
    float mWindScale {0.1f};
    glm::vec3 mWindDirection {1.0f};
    float mPhaseScale {1.0f};

    // 云朵相关
    Texture *mCloudMask{nullptr};
    glm::vec3 mCloudWhiteColor{17.0 / 255, 161.0 / 255, 89.0 / 255};
    glm::vec3 mCloudBlackColor{17.0 / 255, 58.0 / 255,  14.0 / 255};
    float mCloudUvScale {1.0f};
    float mCloudSpeed  {0.1f};
    float mCloudLerp {0.5f};

};

#endif