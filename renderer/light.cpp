#include "light.h"

Light::Light(IActor *parent) :
    EmptyActor(parent), mColor(Qt::white)
{

}

Light::~Light()
{

}

QColor Light::color() const
{
    return mColor;
}

void Light::setColor(const QColor& color)
{
    mColor = color;
}
