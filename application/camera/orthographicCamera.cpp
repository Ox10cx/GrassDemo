#include "orthographicCamera.h"

OrthoGraphicCamera::OrthoGraphicCamera(float left, float right, float bottom, float top, float near, float far)
{
    mLeft = left;
    mRight = right;
    mTop = top;
    mBottom = bottom;
    mNear = near;
    mFar = far;
}
OrthoGraphicCamera::~OrthoGraphicCamera()
{
}
glm::mat4 OrthoGraphicCamera::getProjectionMatrix()
{
    float scale = std::pow(2.0f, mScale);
    return glm::ortho(mLeft * scale, mRight * scale, mBottom * scale, mTop * scale, mNear, mFar);
}

void OrthoGraphicCamera::scale(float dScale)
{
    mScale += dScale;
}