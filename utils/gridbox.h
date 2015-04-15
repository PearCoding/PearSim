#ifndef GRIDBOX_H
#define GRIDBOX_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>

#include "actor.h"

class GridBox : public Actor
{
public:
    GridBox();
    ~GridBox();

    void build(int xc, int yc, int zc, float spacing);
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

#endif // GRIDBOX_H
