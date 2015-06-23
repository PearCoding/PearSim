#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "iactor.h"
#include "data/datagrid.h"
#include "utils/gradient.h"

class Material;
class PS_LIB Grid : public IActor
{
public:
    Grid(IActor* parent = nullptr);
    ~Grid();

    void build(FloatDataGrid* grid, float spacing = 1.0f, float hSpaceing = 1.0f);
    void build(int xc, int yc, float spacing, float hSpaceing);
    void cleanup();

    void draw(Camera *camera, Environment* env);

    inline Gradient<QVector4D>* gradient() const
    {
        return mGradient;
    }

    inline void setGradient(Gradient<QVector4D>* gradient)
    {
        mGradient = gradient;
    }

private:
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mIndexVBO;
    QOpenGLShaderProgram *mProgram;

    int mProjMatrixLoc;
    int mMVMatrixLoc;
    int mHeightLoc;
    int mGradientLoc;
    int mFactorLoc;
    int mHeightBoundaryLoc;

    int mVertexCount;
    int mIndexCount;
    GLfloat* mVertexData;
    GLuint* mIndexData;

    FloatDataGrid* mGrid;
    int mXCount;
    int mYCount;

    QOpenGLTexture mHeightTex;
    QOpenGLTexture mGradientTex;

    Gradient<QVector4D>* mGradient;
};
