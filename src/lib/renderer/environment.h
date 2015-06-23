#pragma once

#include "config.h"
#include <QVector>
#include <QColor>

class Light;
class PS_LIB Environment
{
public:
    Environment();
    ~Environment();

    void addLight(Light* light);
    void removeLight(Light* light);
    Light* light(size_t i);
    size_t lightCount() const;

    void setAmbientColor(QColor& col);
    QColor ambientColor() const;

    void setAmbientFactor(float f);
    float ambientFactor() const;

private:
    QVector<Light*> mLights;
    QColor mAmbientColor;
    float mAmbientFactor;
};

