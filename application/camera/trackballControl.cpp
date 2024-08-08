#include "trackballControl.h"

TrackBallCameraControl::TrackBallCameraControl()
{
}

TrackBallCameraControl::~TrackBallCameraControl()
{
}

void TrackBallCameraControl::onCursor(double xPos, double yPos)
{
    if (mLeftMouseDown)
    {
        // 调整相机参数
        // 经线 维线旋转的增量角度
        float dX = (xPos - mCurrentX) * mSensitivity;
        float dY = (yPos - mCurrentY) * mSensitivity;

        // 分开pitch 和 yaw 各自计算
        pitch(-dY);
        yaw(-dX);
    }
    else if (mMiddleMouseDown)
    {
        float dX = (xPos - mCurrentX) * mMoveSpeed;
        float dY = (yPos - mCurrentY) * mMoveSpeed;

        mCamera->mPosition += mCamera->mUp * dY;
        mCamera->mPosition -= mCamera->mRight * dX;
    }

    mCurrentX = xPos;
    mCurrentY = yPos;
}

void TrackBallCameraControl::onScorll(float offset)
{
    mCamera->scale(mScaleSpeed * offset);
}

void TrackBallCameraControl::pitch(float angle)
{
    // 绕着mRight向量在旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
    //  影响up 和 position  vec4 给到 vec3
    //  向量补0.0f
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);
    // 坐标点补1.0f
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}

void TrackBallCameraControl::yaw(float angle)
{
    // 绕着世界左边细的y轴旋转
    // 绕着mRight向量在旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    //  影响 right / up / position  vec4 给到 vec3
    //  向量补0.0f
    mCamera->mUp = mat * glm::vec4(mCamera->mUp, 0.0f);

    mCamera->mRight = mat * glm::vec4(mCamera->mRight, 0.0f);

    // 坐标点补1.0f
    mCamera->mPosition = mat * glm::vec4(mCamera->mPosition, 1.0f);
}