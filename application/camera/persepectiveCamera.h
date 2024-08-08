#ifndef PERSEPECTIVE_CAMERA_H
#define PERSEPECTIVE_CAMERA_H

#include "camera.h"

class PersepectiveCamera : public Camera
{
public:
    PersepectiveCamera(float fov = 0.0f, float aspect = 0.0f, float near = 0.0f, float far = 0.0f);
    ~PersepectiveCamera();
    glm::mat4 getProjectionMatrix() override;
    void scale(float dScale) override;


private:
    float mFov;
    float mAspect;
};
#endif