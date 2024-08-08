#ifndef CAMERA_H
#define CAMERA_H

#include "../glframework/core.h"

class Camera
{
public:
    Camera();
    ~Camera();

    glm::mat4 getViewMatrix();

    virtual glm::mat4 getProjectionMatrix();
    virtual void scale(float dScale);

public:
    glm::vec3 mPosition{0.0f, 0.0f, 2.0f};
    glm::vec3 mUp{0.0f, 1.0f, 0.0f};
    glm::vec3 mRight{1.0f, 0.0f, 0.0f};

    float mNear {0.0f};
    float mFar  {0.0f};
};

#endif