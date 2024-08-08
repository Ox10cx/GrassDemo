#include "cameraControl.h"

CameraControl::CameraControl()
{
}
CameraControl::~CameraControl()
{
}

void CameraControl::onMouse(int button, int action, double xPos, double yPos)
{
    // 判断当前的按键，是否按下
    bool pressed = action == GLFW_PRESS ? true : false;

    // 如果按下，记录当前按下的位置
    if (pressed)
    {
        mCurrentX = xPos;
        mCurrentY = yPos;
    }

    // 根据按下的鼠标按键不同 更改按键状态
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        mLeftMouseDown = pressed;
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        mRightMouseDown = pressed;
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        mMiddleMouseDown = pressed;
        break;
    }
}
void CameraControl::onCursor(double xPos, double yPos)
{
}
void CameraControl::onKey(int key, int action, int mods)
{
    if (action == GL_REPEAT)
        return;
    // 检测按下或者抬起，给到一个变量
    bool pressed = action == GLFW_PRESS ? true : false;
    mKeyMap[key] = pressed;
}

void CameraControl::update()
{
}

void CameraControl::onScorll(float offset)
{
}