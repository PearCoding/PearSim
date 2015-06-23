#pragma once

#include "config.h"
#include <QColor>

//Semi-real physical shader
class PS_LIB Material
{
public:
    Material();
    ~Material();

    QColor diffuse() const;
    void setDiffuse(const QColor& color);

    QColor specular() const;
    void setSpecular(const QColor& color);

    float smoothness() const;
    void setSmoothness(float f);

private:
    QColor mDiffuse;
    QColor mSpecular;
    float mSmoothness;
};
