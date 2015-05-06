#ifndef MATERIAL_H
#define MATERIAL_H

#include <QColor>

//Semi-real physical shader
class Material
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

#endif // MATERIAL_H
