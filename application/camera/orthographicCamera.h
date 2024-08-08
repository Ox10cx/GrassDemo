#ifndef ORTHO_GRAPHIC_CAMERA_H
#define ORTHO_GRAPHIC_CAMERA_H

#include "camera.h"

class OrthoGraphicCamera : public Camera
{
public:
    OrthoGraphicCamera(float left = 0.0f, float right = 0.0f, float bottom = 0.0f, float top = 0.0f, float near = 0.0f, float far = 0.0f);
    ~OrthoGraphicCamera();
    glm::mat4 getProjectionMatrix() override;

    void scale(float dScale) override;


private:
    float mLeft;
    float mRight;
    float mTop;
    float mBottom;
    float mScale {0.0f};
};

#endif