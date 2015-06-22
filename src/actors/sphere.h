#ifndef SPHERE_H
#define SPHERE_H

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>

#include "iactor.h"

class Shader;
class Material;
class Sphere : public IActor
{
public:
    Sphere(IActor* parent = nullptr);
    ~Sphere();

    void build(float r, unsigned int s, unsigned int segments);
    void cleanup();

    void draw(Camera *camera, Environment* renderer);

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
    int mIndicesCount;
    GLfloat* mVertexData;
    GLuint* mIndexData;

    QColor mColor;
};

#endif // SPHERE_H
