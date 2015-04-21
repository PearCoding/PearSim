#ifndef SPHERE_H
#define SPHERE_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>

#include "iactor.h"

class Sphere : public IActor
{
public:
    Sphere(IActor* parent = nullptr);
    ~Sphere();

    void build(float r, unsigned int s, unsigned int segments);
    void cleanup();

    void draw(Camera *camera);

    void setColor(const QColor& color)
    {
        mColor = color;
    }

    QColor color() const
    {
        return mColor;
    }

private:
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mIndexVBO;
    QOpenGLShaderProgram *mProgram;

    int mProjMatrixLoc;
    int mMVMatrixLoc;
    int mColorLoc;

    int mVertexCount;
    int mIndicesCount;
    GLfloat* mVertexData;
    GLuint* mIndexData;

    QColor mColor;
};

#endif // SPHERE_H
