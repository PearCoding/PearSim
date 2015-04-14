#ifndef GRIDBOX_H
#define GRIDBOX_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

#include "actor.h"

class GridBox : public Actor
{
public:
    GridBox(QOpenGLFunctions* funcs);
    ~GridBox();

    void build(int xc, int yc, int zc, float spacing);
    void cleanup();

    void draw(Camera *camera);
private:
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mIndexVBO;
    QOpenGLShaderProgram *mProgram;
    int mProjMatrixLoc;
    int mMVMatrixLoc;
    int mVertexCount;
    int mIndicesCount;
};

#endif // GRIDBOX_H
