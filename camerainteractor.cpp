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
    const float rotAngle = 10;

    switch(event->key())
    {
    case Qt::Key_W:
        mCamera->move(0,0,-speed);
        break;
    case Qt::Key_S:
        mCamera->move(0,0,speed);
        break;
    case Qt::Key_A:
        mCamera->move(-speed,0,0);
        break;
    case Qt::Key_D:
        mCamera->move(speed,0,0);
        break;
    case Qt::Key_PageUp:
        mCamera->move(0,speed,0);
        break;
    case Qt::Key_PageDown:
        mCamera->move(0,-speed,0);
        break;
    case Qt::Key_X:
        mCamera->reset();
        break;
    case Qt::Key_R:
        mCamera->pitch(rotAngle);
        break;
    case Qt::Key_F:
        mCamera->pitch(-rotAngle);
        break;
    case Qt::Key_T:
        mCamera->yaw(rotAngle);
        break;
    case Qt::Key_G:
        mCamera->yaw(-rotAngle);
        break;
    case Qt::Key_Z:
        mCamera->roll(rotAngle);
        break;
    case Qt::Key_H:
        mCamera->roll(-rotAngle);
        break;
    }
}

void CameraInteractor::mouseMoveEvent(QMouseEvent * event)
{
    const float speed = 0.005f;
    if(event->buttons() & Qt::LeftButton)
    {
        int dx = event->pos().x() - mLastClick.x();
        int dy = event->pos().y() - mLastClick.y();

        //qDebug() << dx << dy;
        mCamera->rotate(dx * speed, dy * speed, 0);
    }
    mLastClick = event->pos();
}

void CameraInteractor::mousePressEvent(QMouseEvent * event)
{
    mLastClick = event->pos();
}

void CameraInteractor::wheelEvent(QWheelEvent * event)
{
     mCamera->move(0,0,event->delta() * 0.001f);
     event->accept();
}
