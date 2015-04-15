#include "gridbox.h"
#include "camera.h"

#include <QOpenGLFunctions>

GridBox::GridBox() :
    Actor(),
    mProgram(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mVertexData(nullptr), mIndexData(nullptr),
    mColor(Qt::white)
{

}

GridBox::~GridBox()
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

void GridBox::build(int xc, int yc, int zc, float spacing)
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

    float xstart = -spacing*xc/2.0f;
    float ystart = -spacing*yc/2.0f;
    float zstart = -spacing*zc/2.0f;
    mVertexCount = 2*((yc+1)*(zc+1) + (xc+1)*(zc+1) + (xc+1)*(yc+1));
    qDebug() << "Vertices " << mVertexCount;
    mVertexData = new GLfloat[mVertexCount*3];

    // X Way
    for (int y = 0; y <= yc; ++y)
    {
        for (int z = 0; z <= zc; ++z)
        {
            const int index = 2*(y*(zc+1) + z)*3;

            mVertexData[index] = xstart;
            mVertexData[index+1] = ystart + y * spacing;
            mVertexData[index+2] = zstart + z * spacing;

            mVertexData[index+3] = xstart + xc * spacing;
            mVertexData[index+4] = ystart + y * spacing;
            mVertexData[index+5] = zstart + z * spacing;
        }
    }

    // Y Way
    for (int x = 0; x <= xc; ++x)
    {
        for (int z = 0; z <= zc; ++z)
        {
            const int index = 2*((yc+1)*(zc+1) + x*(zc+1) + z)*3;
            mVertexData[index] = xstart + x * spacing;
            mVertexData[index+1] = ystart;
            mVertexData[index+2] = zstart + z * spacing;

            mVertexData[index+3] = xstart + x * spacing;
            mVertexData[index+4] = ystart + yc * spacing;
            mVertexData[index+5] = zstart + z * spacing;
        }
    }

    // Z Way
    for (int x = 0; x <= xc; ++x)
    {
        for (int y = 0; y <= yc; ++y)
        {
            const int index = 2*((yc+1)*(zc+1) + (xc+1)*(zc+1) + x*(yc+1) + y)*3;
            mVertexData[index] = xstart + x * spacing;
            mVertexData[index+1] = ystart + y * spacing;
            mVertexData[index+2] = zstart;

            mVertexData[index+3] = xstart + x * spacing;
            mVertexData[index+4] = ystart + y * spacing;
            mVertexData[index+5] = zstart + zc * spacing;
        }
    }

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, mVertexCount * sizeof(GLfloat) * 3);

    mProgram->enableAttributeArray("vertex");
    mProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);

    qDebug() << "Construction finished";
}

void GridBox::cleanup()
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

void GridBox::draw(Camera* camera)
{
    Q_ASSERT(camera);

    mVAO.bind();
    mProgram->bind();
    mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
    mProgram->setUniformValue(mMVMatrixLoc, camera->view() * matrix());
    mProgram->setUniformValue(mColorLoc, mColor);

    glDrawArrays(GL_LINES, 0, mVertexCount);
    //glDrawElements(GL_LINES, mIndicesCount, GL_UNSIGNED_INT, 0);

    mProgram->release();
}
