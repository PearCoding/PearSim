#include "rectangular.h"
#include "camera.h"

#include "renderer/shader.h"

Rectangular::Rectangular(IActor *parent):
    IActor(parent),
    mShader(nullptr), mMaterial(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mColor(Qt::white)
{

}

Rectangular::~Rectangular()
{
    cleanup();
}

GLint Rectangular::sIndicesData[] =
{
    0, 1, 2, 2, 3, 0,
    3, 2, 6, 6, 7, 3,
    7, 6, 5, 5, 4, 7,
    4, 0, 3, 3, 7, 4,
    0, 1, 5, 5, 4, 0,
    1, 5, 6, 6, 2, 1
};

void Rectangular::build(float xw, float yw, float zw)
{
    mVAO.create();
    mVAO.bind();

    const float xw2 = xw / 2;
    const float yw2 = yw / 2;
    const float zw2 = zw / 2;

    int t = 0;
    mVertexData[t+0] = -xw2; mVertexData[t+1] = -yw2; mVertexData[t+2] = zw2;   //Vertex
    mVertexData[t+3] = -1; mVertexData[t+4] = -1; mVertexData[t+5] = 1;         //Normals
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0] =  xw2; mVertexData[t+1] = -yw2; mVertexData[t+2] = zw2;
    mVertexData[t+3] =  1; mVertexData[t+4] = -1; mVertexData[t+5] = 1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0]  =  xw2; mVertexData[t+1]  =  yw2; mVertexData[t+2]  =  zw2;
    mVertexData[t+3]  =  1; mVertexData[t+4]  =  1; mVertexData[t+5]  =  1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0]  = -xw2; mVertexData[t+1] =  yw2; mVertexData[t+2] =  zw2;
    mVertexData[t+3]  = -1; mVertexData[t+4] =  1; mVertexData[t+5] =  1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0] = -xw2; mVertexData[t+1] = -yw2; mVertexData[t+2] = -zw2;
    mVertexData[t+3] = -1; mVertexData[t+4] = -1; mVertexData[t+5] = -1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0] =  xw2; mVertexData[t+1] = -yw2; mVertexData[t+2] = -zw2;
    mVertexData[t+3] =  1; mVertexData[t+4] = -1; mVertexData[t+5] = -1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0] =  xw2; mVertexData[t+1] =  yw2; mVertexData[t+2] = -zw2;
    mVertexData[t+3] =  1; mVertexData[t+4] =  1; mVertexData[t+5] = -1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;                               //UV

    t += 8;
    mVertexData[t+0] = -xw2; mVertexData[t+1] =  yw2; mVertexData[t+2] = -zw2;
    mVertexData[t+3] = -1; mVertexData[t+4] =  1; mVertexData[t+5] = -1;
    mVertexData[t+6] = 0; mVertexData[t+7] = 0;

    t += 8;

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, t * sizeof(GLfloat));

    mIndexVBO.create();
    mIndexVBO.bind();
    mIndexVBO.allocate(sIndicesData, 36 * sizeof(GLint));

    ShaderPreferences prefs;
    prefs.HasAmbient = true;
    prefs.Lights = 1;
    mShader = new Shader;
    mShader->build(prefs);
}

void Rectangular::cleanup()
{
    mIndexVBO.destroy();
    mVBO.destroy();
    mVAO.destroy();

    if(mShader)
    {
        delete mShader;
        mShader = nullptr;
    }
}

void Rectangular::draw(Camera* camera, Environment *env)
{
    Q_ASSERT(camera);

    if(!mMaterial)
    {
        return;
    }

    mVAO.bind();

    mShader->bind(camera->view() * matrix(), camera, mMaterial, env);

    //glDrawArrays(GL_TRIANGLES, 0, 8);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    mShader->release();
}
