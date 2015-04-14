#include "actor.h"

Actor::Actor(QOpenGLFunctions *funcs) :
    mGL(funcs)
{

}

Actor::~Actor()
{

}

void Actor::setMatrix(const QMatrix4x4& mat)
{
    mMatrix = mat;
}

QMatrix4x4 Actor::matrix() const
{
    return mMatrix;
}
