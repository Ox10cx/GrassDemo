#ifndef TRACK_BALL_CONTROL_H
#define TRACK_BALL_CONTROL_H

#include "cameraControl.h"

class TrackBallCameraControl : public CameraControl
{
public:
    TrackBallCameraControl();
    ~TrackBallCameraControl();

    void onCursor(double xPos, double yPos) override;
    void onScorll(float offset) override;

private:
    void pitch(float angle);
    void yaw(float angle);

    private:
    float mMoveSpeed = 0.01f;
};

#endif