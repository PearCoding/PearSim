#include "gridbox.h"
#include "camera.h"

#include <QOpenGLFunctions>

GridBox::GridBox(QOpenGLFunctions *funcs) :
    Actor(funcs),
    mProgram(nullptr)
{

}

GridBox::~GridBox()
{

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
    "out highp vec4 fragColor;\n"
    "void main() {\n"
    "   fragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
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

    mVAO.create();
    mVAO.bind();

    float xstart = -spacing*xc/2.0f;
    float ystart = -spacing*yc/2.0f;
    float zstart = -spacing*zc/2.0f;
    mVertexCount = xc*yc*zc;
    GLfloat* buffer = new GLfloat[mVertexCount*3];

    for (int x = 0; x < xc; ++x)
    {
        for (int y = 0; y < yc; ++y)
        {
            for (int z = 0; z < zc; ++z)
            {
                const int index = x*yc*zc*3 + y*zc*3 + z*3;
                buffer[index] = xstart + x * spacing;
                buffer[index+1] = ystart + y * spacing;
                buffer[index+2] = zstart + z * spacing;
            }
        }
    }

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(buffer, mVertexCount * sizeof(GLfloat) * 3);

    gl()->glEnableVertexAttribArray(0);
    gl()->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    mVBO.release();

    //delete [] buffer;

    //Index
    mIndicesCount = (xc-1)*(yc-1)*(zc-1)* 3 * 2;
    GLint* indices = new GLint[mIndicesCount];

    for (int x = 0; x < xc-1; ++x)
    {
        for (int y = 0; y < yc-1; ++y)
        {
            for (int z = 0; z < zc-1; ++z)
            {
                const int index = x*yc*zc*3*2 + y*zc*3*2 + z*3*2;

                indices[index] = x*yc*zc;
                indices[index+1] = (x+1)*yc*zc;

                indices[index+2] = y*yc*zc;
                indices[index+3] = (y+1)*yc*zc;

                indices[index+4] = z*yc*zc;
                indices[index+5] = (z+1)*yc*zc;
            }
        }
    }

    mIndexVBO.create();
    mIndexVBO.bind();
    mIndexVBO.allocate(indices, mIndicesCount * sizeof(GLint));

    mIndexVBO.release();

    //delete [] indices;

    mVAO.release();
}

void GridBox::cleanup()
{
    mIndexVBO.destroy();
    mVBO.destroy();
    delete mProgram;
    mProgram = nullptr;
}

void GridBox::draw(Camera* camera)
{
    QOpenGLVertexArrayObject::Binder vaoBinder(&mVAO);
    mProgram->bind();
    mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
    mProgram->setUniformValue(mMVMatrixLoc, camera->view() * matrix());

    glDrawElements(GL_LINES, mIndicesCount, GL_UNSIGNED_INT, 0);

    mProgram->release();
}
