#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"
#include "../Object.h"


class PointLight : public Light, public Object
{
public:
    PointLight();
    ~PointLight();

public:
    float mK2 = 1.0f;
    float mK1 = 1.0f;
    float mKc = 1.0f;
};

#endif