#ifndef LIGHT_H
#define LIGHT_H

#include <QVector3D>

class Light
{
public:
    Light();
    ~Light();

    QVector3D position() const;
    void setPosition(const QVector3D& pos);

    QColor color() const;
    void setColor(const QColor& color);

private:
    QVector3D mPosition;
    QColor mColor;
};

#endif // LIGHT_H
