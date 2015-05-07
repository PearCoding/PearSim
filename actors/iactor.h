#ifndef IACTOR_H
#define IACTOR_H

#include <QMatrix4x4>

class Camera;
class Environment;
class IActor
{
public:
    IActor(IActor* parent = nullptr);
    virtual ~IActor();

    void setParent(IActor* actor);
    IActor* parent() const;

    virtual void draw(Camera *camera, Environment* env) = 0;

    void setPosition(const QVector3D& v);
    QVector3D position() const;

    void setRotation(const QVector3D& v);
    QVector3D rotation() const;
    void rotate(const QVector3D& v);
    void rotate(float x, float y, float z);

    void setScale(float f);
    float scale() const;

    QMatrix4x4 matrix() const;

private:
    void cache();

    IActor* mParent;
    QVector3D mPosition;
    QVector3D mRotation;
    float mScale;

    QMatrix4x4 mMatrix;
};

#endif // IACTOR_H
