#include "iactor.h"

#include <QTransform>

IActor::IActor(IActor *parent) :
    mParent(parent), mScale(1)
{
}

IActor::~IActor()
{

}

void IActor::setParent(IActor* actor)
{
    mParent = actor;
}

IActor* IActor::parent() const
{
    return mParent;
}

void IActor::setPosition(const QVector3D& v)
{
    mPosition = v;
    cache();
}

QVector3D IActor::position() const
{
    if(!mParent)
    {
        return mPosition;
    }
    else
    {
        QVector3D rot = mParent->rotation();

        QMatrix4x4 transform;
        transform.rotate(rot.x(), 1, 0, 0);
        transform.rotate(rot.y(), 0, 1, 0);
        transform.rotate(rot.z(), 0, 0, 1);
        transform.scale(mParent->scale());

        return mParent->position() + transform * mPosition;
    }
}

void IActor::setRotation(const QVector3D &v)
{
    mRotation = v;
    mRotation.setX(fmod(mRotation.x(), 360));
    mRotation.setY(fmod(mRotation.y(), 360));
    mRotation.setZ(fmod(mRotation.z(), 360));
    cache();
}

QVector3D IActor::rotation() const
{
    if(!mParent)
    {
        return mRotation;
    }
    else
    {
        return mParent->rotation() + mRotation;
    }
}

void IActor::rotate(const QVector3D& v)
{
    mRotation += v;
    mRotation.setX(fmod(mRotation.x(), 360));
    mRotation.setY(fmod(mRotation.y(), 360));
    mRotation.setZ(fmod(mRotation.z(), 360));

    cache();
}

void IActor::rotate(float x, float y, float z)
{
    rotate(QVector3D(x, y, z));
}

void IActor::setScale(float f)
{
    mScale = f;
    cache();
}

float IActor::scale() const
{
    if(!mParent)
    {
        return mScale;
    }
    else
    {
        return mParent->scale() * mScale;
    }
}

QMatrix4x4 IActor::matrix() const
{
    if(!mParent)
    {
        return mMatrix;
    }
    else
    {
        return mParent->matrix() * mMatrix;
    }
}

void IActor::cache()
{
    QMatrix4x4 m;
    m.setToIdentity();
    m.rotate(mRotation.x(), 1, 0, 0);
    m.rotate(mRotation.y(), 0, 1, 0);
    m.rotate(mRotation.z(), 0, 0, 1);
    m.scale(mScale);
    m.translate(mPosition);

    mMatrix = m;
}
