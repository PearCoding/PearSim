#ifndef RECTANGULAR_H
#define RECTANGULAR_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>

#include "iactor.h"

class Rectangular : public IActor
{
public:
    Rectangular(IActor* parent = nullptr);
    ~Rectangular();

    void build(float xw, float yw, float zw);
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
    GLfloat mVertexData[8*3];

    QColor mColor;

    static GLint sIndicesData[36];
};

#endif // RECTANGULAR_H
