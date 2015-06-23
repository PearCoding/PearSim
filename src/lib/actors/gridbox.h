#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>

#include "iactor.h"

class PS_LIB GridBox : public IActor
{
public:
    GridBox(IActor* parent = nullptr);
    ~GridBox();

    void build(int xc, int yc, int zc, float spacing);
    void cleanup();

    void draw(Camera *camera, Environment* env);

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
