#ifndef ACTOR_H
#define ACTOR_H

#include <QMatrix4x4>

class Camera;
class Actor
{
public:
    Actor();
    virtual ~Actor();

    virtual void draw(Camera *camera) = 0;

    void setMatrix(const QMatrix4x4& mat);
    QMatrix4x4 matrix() const;

private:
    QMatrix4x4 mMatrix;
};

#endif // ACTOR_H
