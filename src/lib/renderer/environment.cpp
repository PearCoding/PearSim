#include "environment.h"

#include "light.h"

Environment::Environment() :
    mAmbientColor(Qt::white), mAmbientFactor(0)
{
}

Environment::~Environment()
{
    qDeleteAll(mLights);
}

void Environment::addLight(Light* light)
{
    mLights.append(light);
}

void Environment::removeLight(Light* light)
{
    mLights.removeOne(light);
}

Light* Environment::light(size_t i)
{
    return mLights.at(i);
}

size_t Environment::lightCount() const
{
    return mLights.size();
}

void Environment::setAmbientColor(QColor& col)
{
    mAmbientColor = col;
}

QColor Environment::ambientColor() const
{
    return mAmbientColor;
}

void Environment::setAmbientFactor(float f)
{
    mAmbientFactor = f;
}

float Environment::ambientFactor() const
{
    return mAmbientFactor;
}
