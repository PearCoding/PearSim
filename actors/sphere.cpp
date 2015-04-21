#include "sphere.h"
#include "camera.h"

#include <math.h>

#ifndef M_PI
# define M_PI (3.14159265358)
# define M_PI_2 (1.57079632679)
#endif

Sphere::Sphere(IActor *parent) :
    IActor(parent),
    mProgram(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mVertexData(nullptr), mIndexData(nullptr),
    mColor(Qt::white)
{

}

Sphere::~Sphere()
{
    cleanup();
}

static const char *vertexShaderSourceCore =
        "#version 150\n"
        "in vec4 vertex;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSourceCore =
        "#version 150\n"
        "uniform vec4 color;\n"
        "out highp vec4 fragColor;\n"
        "void main() {\n"
        "   fragColor = color;\n"
        "}\n";

void Sphere::build(float r, unsigned int slices, unsigned int segments)
{
    mProgram = new QOpenGLShaderProgram;
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCore);
    mProgram->bindAttributeLocation("vertex", 0);
    mProgram->link();

    mProgram->bind();
    mProjMatrixLoc = mProgram->uniformLocation("projMatrix");
    mMVMatrixLoc = mProgram->uniformLocation("mvMatrix");
    mColorLoc = mProgram->uniformLocation("color");

    mVAO.create();
    mVAO.bind();

    mVertexCount = slices*segments;
    qDebug() << "Vertices " << mVertexCount;
    mVertexData = new GLfloat[mVertexCount*3];

    const float R = 1/(float)(slices - 1);
    const float S = 1/(float)(segments - 1);

    int v = 0;
    for(unsigned int i = 0; i < slices; ++i)
    {
        for(unsigned int j = 0; j < segments; ++j)
        {
            mVertexData[v++] = r * cos(2 * M_PI * j * S) * sin( M_PI * i * R );
            mVertexData[v++] = r * sin( -M_PI_2 + M_PI * i * R );
            mVertexData[v++] = r * sin(2 * M_PI * j * S) * sin( M_PI * i * R );
        }
    }

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, mVertexCount * sizeof(GLfloat) * 3);

    mProgram->enableAttributeArray("vertex");
    mProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);

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

    qDebug() << "Construction finished";
}

void Sphere::cleanup()
{
    mIndexVBO.destroy();
    mVBO.destroy();
    mVAO.destroy();

    if(mProgram)
    {
        delete mProgram;
        mProgram = nullptr;
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

void Sphere::draw(Camera* camera)
{
    Q_ASSERT(camera);

    mVAO.bind();
    mProgram->bind();
    mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
    mProgram->setUniformValue(mMVMatrixLoc, camera->view() * matrix());
    mProgram->setUniformValue(mColorLoc, mColor);

    glDrawElements(GL_TRIANGLES, mIndicesCount, GL_UNSIGNED_INT, 0);

    mProgram->release();
}

