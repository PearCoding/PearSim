#include "camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::setProjection(const QMatrix4x4& mat)
{
    mProjection = mat;
}

QMatrix4x4 Camera::projection() const
{
    return mProjection;
}

void Camera::setView(const QMatrix4x4& mat)
{
    mView = mat;
}

QMatrix4x4 Camera::view() const
{
    return mView;
}

void Camera::reset()
{
    mView.setToIdentity();
}

void Camera::translate(float x, float y, float z)
{
    mView.translate(x, y, z);
}

void Camera::rotate(const QQuaternion& quat)
{
    mView.rotate(quat);
}

void Camera::lookAt(const QVector3D& eye,
                    const QVector3D& center,
                    const QVector3D& up)
{
    mView.lookAt(eye, center, up);
}
