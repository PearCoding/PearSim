#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "iactor.h"

class Shader;
class Material;
class PS_LIB Mesh : public IActor
{
public:
    Mesh(IActor* parent = nullptr);
    ~Mesh();

    void cleanup();

    void load(GLfloat* vertices, int vertexCount, GLuint* indices, int indexCount);

    void draw(Camera *camera, Environment* env);

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
    int mIndexCount;
    GLfloat* mVertexData;
    GLuint* mIndexData;
};
