#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>
#include <QColor>

#include "actors/emptyactor.h"

class Light : public EmptyActor
{
public:
    Light(IActor* parent = nullptr);
    virtual ~Light();

    QColor color() const;
    void setColor(const QColor& color);

private:
    QVector3D mPosition;
    QColor mColor;
};

#endif // LIGHT_H
