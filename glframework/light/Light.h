#ifndef LIGHT_H
#define LIGHT_H

#include "../core.h"

class Light
{
public:
    Light();
    ~Light();

public:
    // 关照强度
    glm::vec3 mColor{1.0f};
    // 高光反射的强度
    float mSpecularIntensity{1.0f};
};

#endif