#include "light.h"

Light::Light() :
    mPosition(), mColor(Qt::white)
{

}

Light::~Light()
{

}

QVector3D Light::position() const
{
    return mPosition;
}

void Light::setPosition(const QVector3D& pos)
{
    mPosition = pos;
}

QColor Light::color() const
{
    return mColor;
}

void Light::setColor(const QColor& color)
{
    mColor = color;
}
