#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <QVector>
#include <QColor>

class Light;
class Environment
{
public:
    Environment();
    ~Environment();

    void addLight(Light* light);
    void removeLight(Light* light);
    Light* light(size_t i);

    void setAmbientColor(QColor& col);
    QColor ambientColor() const;

    void setAmbientFactor(float f);
    float ambientFactor() const;

private:
    QVector<Light*> mLights;
    QColor mAmbientColor;
    float mAmbientFactor;
};

#endif // ENVIRONMENT_H
