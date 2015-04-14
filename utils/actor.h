#ifndef ACTOR_H
#define ACTOR_H

#include <QMatrix4x4>

class Camera;
class QOpenGLFunctions;
class Actor
{
public:
    Actor(QOpenGLFunctions* funcs);
    virtual ~Actor();

    virtual void draw(Camera *camera) = 0;

    void setMatrix(const QMatrix4x4& mat);
    QMatrix4x4 matrix() const;
protected:
    QOpenGLFunctions* gl()
    {
        return mGL;
    }

private:
    QOpenGLFunctions* mGL;
    QMatrix4x4 mMatrix;
};

#endif // ACTOR_H
