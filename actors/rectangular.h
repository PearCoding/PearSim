#ifndef RECTANGULAR_H
#define RECTANGULAR_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QColor>

#include "iactor.h"

class Shader;
class Material;
class Rectangular : public IActor
{
public:
    Rectangular(IActor* parent = nullptr);
    ~Rectangular();

    void build(float xw, float yw, float zw);
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

    Material* material() const
    {
        return mMaterial;
    }

    void setMaterial(Material* material)
    {
        mMaterial = material;
    }

private:
    QOpenGLVertexArrayObject mVAO;
    QOpenGLBuffer mVBO;
    QOpenGLBuffer mIndexVBO;

    Shader* mShader;
    Material* mMaterial;

    int mVertexCount;
    GLfloat mVertexData[24*8];

    QColor mColor;

    static GLint sIndicesData[36];
};

#endif // RECTANGULAR_H
