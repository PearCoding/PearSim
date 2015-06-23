#pragma once

#include "iinteractor.h"

#include <QPoint>

class Camera;
class PS_LIB CameraInteractor : public IInteractor
{
public:
    CameraInteractor(Camera* cam);
    virtual ~CameraInteractor();

    virtual void keyPressEvent(QKeyEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
private:
    Camera* mCamera;

    QPoint mLastClick;
};