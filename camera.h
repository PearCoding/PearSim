#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    ~Camera();

    void setProjection(const QMatrix4x4& mat);
    QMatrix4x4 projection() const;

    void setView(const QMatrix4x4& mat);
    QMatrix4x4 view() const;

    void reset();
    void translate(float x, float y, float z);
    void rotate(const QQuaternion& quat);
    void lookAt(const QVector3D& eye,
                const QVector3D& center,
                const QVector3D& up);

private:
    QMatrix4x4 mProjection;
    QMatrix4x4 mView;
};

#endif // CAMERA_H
