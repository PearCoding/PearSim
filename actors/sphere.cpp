#include "sphere.h"
#include "camera.h"
#include "renderer/shader.h"

#include <math.h>

#ifndef M_PI
# define M_PI (3.14159265358)
# define M_PI_2 (1.57079632679)
#endif

Sphere::Sphere(IActor *parent) :
    IActor(parent),
    mShader(nullptr), mMaterial(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mVertexData(nullptr), mIndexData(nullptr),
    mColor(Qt::white)
{

}

Sphere::~Sphere()
{
    cleanup();
}

void Sphere::build(float r, unsigned int slices, unsigned int segments)
{
    mVAO.create();
    mVAO.bind();

    mVertexCount = slices*segments;
    qDebug() << "Vertices " << mVertexCount;
    mVertexData = new GLfloat[mVertexCount*(3+3+2)];

    const float R = 1/(float)(slices - 1);
    const float S = 1/(float)(segments - 1);

    int v = 0;
    for(unsigned int i = 0; i < slices; ++i)
    {
        for(unsigned int j = 0; j < segments; ++j)
        {
            float x = sin(2 * M_PI * j * S) * sin( M_PI * i * R );
            float y = sin( -M_PI_2 + M_PI * i * R );
            float z = cos(2 * M_PI * j * S) * sin( M_PI * i * R );

            //Vertices
            mVertexData[v++] = r * x;
            mVertexData[v++] = r * y;
            mVertexData[v++] = r * z;

            //Normals
            mVertexData[v++] = x;
            mVertexData[v++] = y;
            mVertexData[v++] = z;

            //UVs TODO
            mVertexData[v++] = x;
            mVertexData[v++] = z;
        }
    }

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, mVertexCount * sizeof(GLfloat) * (3+3+2));

    mIndicesCount = slices*segments*6;
    mIndexData = new GLuint[mIndicesCount];

    int c = 0;
    for(unsigned int i = 0; i < slices; ++i)
    {
        for(unsigned int j = 0; j < segments; ++j)
        {
            mIndexData[c++] = j + segments * i;
            mIndexData[c++] = j+1 + segments * i;
            mIndexData[c++] = j + segments*(i+1);
            mIndexData[c++] = j+1 + segments*(i+1);
            mIndexData[c++] = j + segments*(i+1);
            mIndexData[c++] = j+1 + segments * (i);
        }
    }

    mIndexVBO.create();
    mIndexVBO.bind();
    mIndexVBO.allocate(mIndexData, mIndicesCount*sizeof(GLuint));

    ShaderPreferences prefs;
    prefs.HasAmbient = true;
    prefs.Lights = 1;
    mShader = new Shader;
    mShader->build(prefs);
}

void Sphere::cleanup()
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

void Sphere::draw(Camera* camera, Environment* env)
{
    if(!mMaterial)
    {
        return;
    }

    Q_ASSERT(camera);

    mVAO.bind();
    mShader->bind(camera->view() * matrix(), camera, mMaterial, env);

    glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);

    mShader->release();
}

