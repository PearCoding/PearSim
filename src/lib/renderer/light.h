#pragma once

#include <QVector3D>
#include <QColor>

#include "actors/emptyactor.h"

class PS_LIB Light : public EmptyActor
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

