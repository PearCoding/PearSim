#include "material.h"

Material::Material()
{

}

Material::~Material()
{

}

QColor Material::diffuse() const
{
    return mDiffuse;
}

void Material::setDiffuse(const QColor& color)
{
    mDiffuse = color;
}

QColor Material::specular() const
{
    return mSpecular;
}

void Material::setSpecular(const QColor& color)
{
    mSpecular = color;
}

float Material::smoothness() const
{
    return mSmoothness;
}

void Material::setSmoothness(float f)
{
    mSmoothness = f;
}
