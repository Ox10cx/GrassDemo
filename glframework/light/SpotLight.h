#ifndef SPOT_LIGHT_H
#define SPOT_LIGHT_H

#include "Light.h"
#include "../Object.h"


class SpotLight : public Light, public Object
{
public:
    SpotLight();
    ~SpotLight();

public:
   glm::vec3 mTargetDirection{-1.0f};
   float mInnerAngle {0.0f};
   float mOuterAngle{0.0f};
};

#endif