#include "iactor.h"

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
    return mPosition;
}

void IActor::setRotation(const QVector3D &v)
{
    mRotation = v;
    mRotation.setX(fmod(mRotation.x(), 180));
    mRotation.setY(fmod(mRotation.y(), 180));
    mRotation.setZ(fmod(mRotation.z(), 180));
    cache();
}

QVector3D IActor::rotation() const
{
    return mRotation;
}

void IActor::rotate(const QVector3D& v)
{
    mRotation += v;
    mRotation.setX(fmod(mRotation.x(), 180));
    mRotation.setY(fmod(mRotation.y(), 180));
    mRotation.setZ(fmod(mRotation.z(), 180));

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
    return mScale;
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
