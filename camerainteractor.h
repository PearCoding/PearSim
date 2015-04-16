#ifndef CAMERAINTERACTOR_H
#define CAMERAINTERACTOR_H

#include "iinteractor.h"

#include <QPointF>

class Camera;
class CameraInteractor : public IInteractor
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

#endif // CAMERAINTERACTOR_H
