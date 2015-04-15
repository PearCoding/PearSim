#include "camerainteractor.h"
#include "camera.h"

#include <QKeyEvent>

CameraInteractor::CameraInteractor(Camera* cam) :
    mCamera(cam)
{
}

CameraInteractor::~CameraInteractor()
{
}

void CameraInteractor::keyPressEvent(QKeyEvent * event)
{
    const float speed = 0.01f;
    switch(event->key())
    {
    case Qt::Key_W:
        mCamera->translate(0,0,speed);
        break;
    case Qt::Key_S:
        mCamera->translate(0,0,-speed);
        break;
    case Qt::Key_A:
        mCamera->translate(speed,0,0);
        break;
    case Qt::Key_D:
        mCamera->translate(-speed,0,0);
        break;
    case Qt::Key_PageUp:
        mCamera->translate(0,-speed,0);
        break;
    case Qt::Key_PageDown:
        mCamera->translate(0,speed,0);
        break;
    }
}

void CameraInteractor::mouseMoveEvent(QMouseEvent * event)
{

}

void CameraInteractor::mousePressEvent(QMouseEvent * event)
{

}

void CameraInteractor::mouseReleaseEvent(QMouseEvent * event)
{

}

void CameraInteractor::wheelEvent(QWheelEvent * event)
{

}
