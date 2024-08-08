#ifndef GAME_CAMERA_CONTROL_H
#define GAME_CAMERA_CONTROL_H

#include "cameraControl.h"

class GameCameraControl : public CameraControl
{
public:
    GameCameraControl();
    ~GameCameraControl();

    void onCursor(double xPos, double yPos) override;
    void update() override;

private:
    void pitch(float angle);
    void yaw(float angle);

private:
   float mPitch {0.0f};
   float mSpeed {0.1f};


};

#endif