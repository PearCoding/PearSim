#include "mesh.h"
#include "camera.h"
#include "renderer/shader.h"

Mesh::Mesh(IActor *parent) :
    IActor(parent),
    mShader(nullptr), mMaterial(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mVertexData(nullptr), mIndexData(nullptr)
{
}

Mesh::~Mesh()
{
    cleanup();
}

void Mesh::load(GLfloat* vertices, int vertexCount, GLuint* indices, int indexCount)
{
    Q_ASSERT(vertices && indices);

    mVertexData = vertices;
    mVertexCount = vertexCount;

    mIndexData = indices;
    mIndexCount = indexCount;

    mVAO.create();
    mVAO.bind();

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, vertexCount * 8 * sizeof(GLfloat));

    mIndexVBO.create();
    mIndexVBO.bind();
    mIndexVBO.allocate(mIndexData, indexCount * sizeof(GLuint));

    ShaderPreferences prefs;
    prefs.HasAmbient = true;
    prefs.Lights = 1;
    mShader = new Shader;
    mShader->build(prefs);
}

void Mesh::cleanup()
{
    mIndexVBO.destroy();
    mVBO.destroy();
    mVAO.destroy();

    if(mShader)
    {
        delete mShader;
        mShader = nullptr;
    }

    if(mVertexData)
    {
        delete [] mVertexData;
        mVertexData = nullptr;
    }

    if(mIndexData)
    {
        delete [] mIndexData;
        mIndexData = nullptr;
    }
}

void Mesh::draw(Camera* camera, Environment *env)
{
    Q_ASSERT(camera);

    if(!mMaterial && ! mVertexData)
    {
        return;
    }

    mVAO.bind();

    mShader->bind(camera->view() * matrix(), camera, mMaterial, env);

    glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_INT, 0);

    mShader->release();
}
