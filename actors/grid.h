#ifndef GRID_H
#define GRID_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "iactor.h"
#include "datagrid.h"

class Shader;
class Material;
class Grid : public IActor
{
public:
    Grid(IActor* parent = nullptr);
    ~Grid();

    void build(FloatDataGrid* grid, float spacing = 1.0f, float hSpaceing = 1.0f);
    void build(int xc, int yc, float spacing, float hSpaceing);
    void cleanup();

    void draw(Camera *camera, Environment* env);

    Material* material() const
    {
        return mMaterial;
    }

    void setMaterial(Material* material)
    {
        mMaterial = material;
    }

    Material* contourMaterial() const
    {
        return mContourMaterial;
    }

    void setContourMaterial(Material* material)
    {
        mContourMaterial = material;
    }
private:
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mIndexVBO;

    Shader* mShader;
    Material* mMaterial;
    Material* mContourMaterial;

    int mVertexCount;
    int mIndexCount;
    GLfloat* mVertexData;
    GLuint* mIndexData;

    FloatDataGrid* mGrid;
    int mXCount;
    int mYCount;
};

#endif // GRID_H
