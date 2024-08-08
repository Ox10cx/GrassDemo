#ifndef CAMERA_CONTROL_H
#define CAMERA_CONTROL_H

#include "../glframework/core.h"
#include "camera.h"
#include <map>

class CameraControl
{
public:
    CameraControl();
    ~CameraControl();

    virtual void onMouse(int button, int action, double xPos, double yPos);
    virtual void onCursor(double xPos, double yPos);
    virtual void onKey(int key, int action, int mods);

    virtual void update();

    virtual void onScorll(float offset);

    void setCamera(Camera* camera) {mCamera = camera;}
    void setSensitivity(float s) {mSensitivity = s;}
    void setScaleSpeed(float s) {mScaleSpeed = s;}


protected:
    // 鼠标按键状态
    bool mLeftMouseDown = false;
    bool mRightMouseDown = false;
    bool mMiddleMouseDown = false;

    // 当前鼠标的位置
    float mCurrentX = 0.0f;
    float mCurrentY = 0.0f;

    // 敏感度
    float mSensitivity = 0.2f;

    Camera* mCamera = nullptr;

    std::map<int, bool> mKeyMap;

    float mScaleSpeed = 0.2f;
    
};
#endif
