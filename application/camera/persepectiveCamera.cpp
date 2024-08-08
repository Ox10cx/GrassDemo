#include "persepectiveCamera.h"

PersepectiveCamera::PersepectiveCamera(float fov, float aspect, float near, float far)
{
    mFov = fov;
    mAspect = aspect;
    mNear = near;
    mFar = far;
}
PersepectiveCamera::~PersepectiveCamera()
{
}
glm::mat4 PersepectiveCamera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(mFov), mAspect, mNear, mFar);
}

void PersepectiveCamera::scale(float dScale)
{
    auto front = glm::cross(mUp, mRight);
    mPosition += (front * dScale);
}