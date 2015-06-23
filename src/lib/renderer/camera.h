#pragma once

#include "config.h"
#include <QMatrix4x4>

class PS_LIB Camera
{
public:
    Camera();
    ~Camera();

    void setProjection(const QMatrix4x4& mat);
    QMatrix4x4 projection() const;

    void setView(const QMatrix4x4& mat);
    QMatrix4x4 view() const;

    void reset();

    void move(float x, float y, float z);
    void moveGlobal(float x, float y, float z);

    void rotate(const QQuaternion& quat);
    void rotate(float yaw, float pitch, float roll);

    void lookAt(const QVector3D& eye,
                const QVector3D& center,
                const QVector3D& up);

    QVector3D focalPoint() const;
    void setFocalPoint(const QVector3D& v);

    QVector3D position() const;
    void setPosition(const QVector3D& v);

    QVector3D upVector() const;

    //In rad
    void pitch(float angle);
    void yaw(float angle);
    void roll(float angle);

private:
    void recalculateMatrix();

    QMatrix4x4 mProjection;
    QMatrix4x4 mView;

    //Positions
    QVector3D mFocalPoint;
    QVector3D mPosition;

    //Direction
    QVector3D mUpVector;
};
