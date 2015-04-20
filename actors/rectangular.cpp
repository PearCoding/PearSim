#include "rectangular.h"
#include "camera.h"

Rectangular::Rectangular(IActor *parent):
    IActor(parent),
    mProgram(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mColor(Qt::white)
{

}

Rectangular::~Rectangular()
{

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

void Rectangular::build(float xw, float yw, float zw)
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

    const float xw2 = xw / 2;
    const float yw2 = yw / 2;
    const float zw2 = zw / 2;

    mVertexData[0]  = -xw2; mVertexData[1]  = -yw2; mVertexData[2]  =  zw2;
    mVertexData[3]  =  xw2; mVertexData[4]  = -yw2; mVertexData[5]  =  zw2;
    mVertexData[6]  =  xw2; mVertexData[7]  =  yw2; mVertexData[8]  =  zw2;
    mVertexData[9]  = -xw2; mVertexData[10] =  yw2; mVertexData[11] =  zw2;
    mVertexData[12] = -xw2; mVertexData[13] = -yw2; mVertexData[14] = -zw2;
    mVertexData[15] =  xw2; mVertexData[16] = -yw2; mVertexData[17] = -zw2;
    mVertexData[18] =  xw2; mVertexData[19] =  yw2; mVertexData[20] = -zw2;
    mVertexData[21] = -xw2; mVertexData[22] =  yw2; mVertexData[23] = -zw2;

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, 8 * sizeof(GLfloat) * 3);

    mProgram->enableAttributeArray("vertex");
    mProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3);

    mIndexVBO.create();
    mIndexVBO.bind();
    mIndexVBO.allocate(sIndicesData, 36 * sizeof(GLint));

    qDebug() << "Construction finished";
}

void Rectangular::cleanup()
{
    mIndexVBO.destroy();
    mVBO.destroy();
    mVAO.destroy();

    if(mProgram)
    {
        delete mProgram;
        mProgram = nullptr;
    }
}

void Rectangular::draw(Camera* camera)
{
    Q_ASSERT(camera);

    mVAO.bind();
    mProgram->bind();
    mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
    mProgram->setUniformValue(mMVMatrixLoc, camera->view() * matrix());
    mProgram->setUniformValue(mColorLoc, mColor);

    //glDrawArrays(GL_TRIANGLES, 0, 8);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    mProgram->release();
}
