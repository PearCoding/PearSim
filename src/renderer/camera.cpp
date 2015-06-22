#include "camera.h"

//TODO: Always using recalculateMatrix() after little operations is not fast
Camera::Camera()
{
    reset();
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
    mFocalPoint = QVector3D(0,0,1);
    mPosition = QVector3D(0,0,0);
    mUpVector = QVector3D(0,1,0);
}

void Camera::move(float x, float y, float z)
{
    QVector3D offset(x,y,z);

    QMatrix4x4 rot;
    rot.lookAt(QVector3D(0,0,0), mFocalPoint - mPosition, mUpVector);

    offset = rot * offset;

    mFocalPoint += offset;
    mPosition += offset;

    recalculateMatrix();
}

void Camera::moveGlobal(float x, float y, float z)
{
    mFocalPoint += QVector3D(x,y,z);
    mPosition += QVector3D(x,y,z);

    recalculateMatrix();
}

void Camera::rotate(const QQuaternion& quat)
{
    mFocalPoint = quat.rotatedVector(mFocalPoint - mPosition) + mPosition;
    mUpVector = quat.rotatedVector(mUpVector);

    recalculateMatrix();
}

void Camera::rotate(float yaw, float pitch, float roll)
{
    QVector3D viewDir = mFocalPoint - mPosition;

    //roll
    QQuaternion rot = QQuaternion(roll, viewDir);
    rot.normalize();

    //viewDir = rot.rotatedVector(viewDir);
    mUpVector = rot.rotatedVector(mUpVector);

    //pitch
    QVector3D right = QVector3D::crossProduct(viewDir, mUpVector);

    rot = QQuaternion(pitch, right);
    rot.normalize();

    viewDir = rot.rotatedVector(viewDir);
    mUpVector = rot.rotatedVector(mUpVector);

    //yaw
    rot = QQuaternion(yaw, mUpVector);
    rot.normalize();

    mFocalPoint = rot.rotatedVector(viewDir) + mPosition;

    recalculateMatrix();
}

void Camera::lookAt(const QVector3D& eye,
                    const QVector3D& center,
                    const QVector3D& up)
{
    mPosition = eye;
    mFocalPoint = center;
    mUpVector = up;

    recalculateMatrix();
}

QVector3D Camera::focalPoint() const
{
    return mFocalPoint;
}

void Camera::setFocalPoint(const QVector3D& v)
{
    mFocalPoint = v;
    recalculateMatrix();
}

QVector3D Camera::position() const
{
    return mPosition;
}

void Camera::setPosition(const QVector3D& v)
{
    mPosition = v;
    recalculateMatrix();
}

QVector3D Camera::upVector() const
{
    return mUpVector;
}

void Camera::pitch(float angle)
{
    QVector3D viewDir = mFocalPoint - mPosition;
    QVector3D right = QVector3D::crossProduct(viewDir, mUpVector);

    QQuaternion rot(angle, right);
    rot.normalize();

    mFocalPoint = rot.rotatedVector(viewDir) + mPosition;
    mUpVector = rot.rotatedVector(mUpVector);

    recalculateMatrix();
}

void Camera::yaw(float angle)
{
    QQuaternion rot(angle, mUpVector);
    rot.normalize();

    mFocalPoint = rot.rotatedVector(mFocalPoint - mPosition) + mPosition;

    recalculateMatrix();
}

void Camera::roll(float angle)
{
    QVector3D viewDir = mFocalPoint - mPosition;
    QQuaternion rot(angle, viewDir);
    rot.normalize();

    //mFocalPoint = rot.rotatedVector(viewDir) + mPosition;
    mUpVector = rot.rotatedVector(mUpVector);

    recalculateMatrix();
}

void Camera::recalculateMatrix()
{
    mView.setToIdentity();
    mView.lookAt(mPosition, mFocalPoint, mUpVector);
}
