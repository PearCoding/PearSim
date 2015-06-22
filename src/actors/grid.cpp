#include "grid.h"
#include "renderer/shader.h"
#include "renderer/camera.h"

#include <QColor>

Grid::Grid(IActor *parent) :
    IActor(parent),
    mProgram(nullptr), mProjMatrixLoc(0), mMVMatrixLoc(0),
    mHeightLoc(0), mGradientLoc(0), mFactorLoc(0), mHeightBoundaryLoc(0),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mVertexData(nullptr), mIndexData(nullptr),
    mGrid(nullptr), mXCount(0), mYCount(0),
    mHeightTex(QOpenGLTexture::Target2D), mGradientTex(QOpenGLTexture::Target1D),
    mGradient(nullptr)
{
}

Grid::~Grid()
{
    cleanup();
}

static const char *vertexShaderSourceCore =
        "#version 150\n"
        "in vec4 vertex;\n"
        "in mediump vec3 normal;\n"
        "in lowp vec2 uv;\n"
        "out mediump vec3 N;\n"
        "out lowp vec2 Tex;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   N = (mvMatrix*vec4(normal,0)).xyz;\n"
        "   Tex = uv;\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

static const char *fragmentShaderSourceCore =
        "#version 150\n"
        "in mediump vec3 N;\n"
        "in lowp vec2 Tex;\n"
        "out highp vec4 fragColor;\n"
        "uniform vec2 heightBoundary;\n"
        "uniform sampler2D heightMap;\n"
        "uniform sampler1D gradientMap;\n"
        "uniform float factor;\n"
        "void main() {\n"
        "   float t = (texture(heightMap, Tex).r-heightBoundary.x)/(heightBoundary.y - heightBoundary.x);\n"
        "   fragColor = factor*texture(gradientMap, t);\n"
        "}\n";

void Grid::build(FloatDataGrid* grid, float spacing, float hSpaceing)
{
    //Q_ASSERT(grid);

    if(mGrid && mXCount != 0 && grid->width() == mXCount &&
            mYCount != 0 && grid->height())//Just update values
    {
        mGrid = grid;

        //TODO: Optimize
        build(grid->width(), grid->height(), spacing, hSpaceing);
    }
    else
    {
        mGrid = grid;
        build(grid->width(), grid->height(), spacing, hSpaceing);
    }
}

void Grid::build(int xc, int yc, float spacing, float hSpaceing)
{
    Q_ASSERT(xc > 1 && yc > 1 && spacing != 0);

    if(mXCount != 0 || mYCount != 0)
    {
        cleanup();
    }

    mXCount = xc;
    mYCount = yc;

    mVertexCount = xc*yc;
    mVertexData = new GLfloat[mVertexCount*8];

    const float sX = -spacing*xc/2.0f;
    const float sY = -spacing*yc/2.0f;

    const float max = mGrid->max();
    const float min = mGrid->min();

    qDebug() << sX << sY << max << min;
    for(int y = 0; y < yc; ++y)
    {
        const float ty = y/(float)yc;
        for(int x = 0; x < xc; ++x)
        {
            const float tx = x/(float)xc;
            const int index = x*8+y*xc*8;

            float height = 0;
            if(mGrid && max != min)
            {
                height = (mGrid->at(x,y)-min)/(max-min);
                height = !std::isfinite(height) ? 0 : height;
            }

            mVertexData[index+0] = x*spacing+sX;
            mVertexData[index+1] = height*hSpaceing;
            mVertexData[index+2] = y*spacing+sY;

            mVertexData[index+3] = 0; mVertexData[index+4] = 0; mVertexData[index+5] = 1;//TODO
            mVertexData[index+6] = tx; mVertexData[index+7] = ty;
        }
    }

    mIndexCount = xc*yc + (xc-1)*(yc-2);
    mIndexData = new GLuint[mIndexCount];

    int i = 0;
    for(int y = 0; y < yc-1; ++y)
    {
        if((y&1) == 0)
        {
            for(int x = 0; x < xc; ++x)
            {
                mIndexData[i++] = x + y*xc;
                mIndexData[i++] = x + (y + 1)*xc;
            }
        }
        else
        {
            for(int x = xc-1; x > 0; --x)
            {
                mIndexData[i++] = x + (y + 1)*xc;
                mIndexData[i++] = x - 1 + y*xc;
            }
        }
    }

    if((yc&1) && yc > 2)
    {
        mIndexData[i++] = (yc-1)*xc;
    }

    // OpenGL code
    mVAO.create();
    mVAO.bind();

    mVBO.create();
    mVBO.bind();
    mVBO.allocate(mVertexData, mVertexCount * sizeof(GLfloat) * 8);

    mIndexVBO.create();
    mIndexVBO.bind();
    mIndexVBO.allocate(mIndexData, mIndexCount * sizeof(GLuint));

    qDebug() << "Vertices " << mVertexCount << " Indices " << mIndexCount;
    mProgram = new QOpenGLShaderProgram;
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCore);
    mProgram->bindAttributeLocation("vertex", 0);
    mProgram->bindAttributeLocation("normal", 1);
    mProgram->bindAttributeLocation("uv", 2);
    mProgram->link();

    mProgram->bind();
    mProjMatrixLoc = mProgram->uniformLocation("projMatrix");
    mMVMatrixLoc = mProgram->uniformLocation("mvMatrix");
    mHeightLoc = mProgram->uniformLocation("heightMap");
    mGradientLoc = mProgram->uniformLocation("gradientMap");
    mFactorLoc = mProgram->uniformLocation("factor");
    mHeightBoundaryLoc = mProgram->uniformLocation("heightBoundary");

    mProgram->enableAttributeArray("vertex");
    mProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 8*sizeof(GLfloat));

    mProgram->enableAttributeArray("normal");
    mProgram->setAttributeBuffer("normal", GL_FLOAT, 3*sizeof(GLfloat), 3, 8*sizeof(GLfloat));

    mProgram->enableAttributeArray("uv");
    mProgram->setAttributeBuffer("uv", GL_FLOAT, 6*sizeof(GLfloat), 2, 8*sizeof(GLfloat));

    mProgram->setUniformValue(mHeightBoundaryLoc, mGrid->min(), mGrid->max());
    mProgram->setUniformValue(mHeightLoc, (uint)0);
    mProgram->setUniformValue(mGradientLoc, (uint)1);

    mHeightTex.setFormat(QOpenGLTexture::R32F);
    mHeightTex.setMinificationFilter(QOpenGLTexture::Linear);
    mHeightTex.setMagnificationFilter(QOpenGLTexture::Linear);
    mHeightTex.setWrapMode(QOpenGLTexture::ClampToEdge);
    mHeightTex.setSize(mXCount, mYCount);
    mHeightTex.allocateStorage();

    if(mGrid)
    {
        mHeightTex.bind();
        mHeightTex.setData(QOpenGLTexture::Red, QOpenGLTexture::Float32, mGrid->ptr());
    }

    const int GRADIENT_QUALITY = 256;
    mGradientTex.setFormat(QOpenGLTexture::RGBA8_UNorm);
    mGradientTex.setMinificationFilter(QOpenGLTexture::Linear);
    mGradientTex.setMagnificationFilter(QOpenGLTexture::Linear);
    mGradientTex.setWrapMode(QOpenGLTexture::ClampToEdge);
    mGradientTex.setSize(GRADIENT_QUALITY);
    mGradientTex.allocateStorage();

    unsigned char gradientData[GRADIENT_QUALITY*4];
    if(mGradient)
    {
        for(int i = 0; i < GRADIENT_QUALITY; ++i)
        {
            QVector4D col = mGradient->value(i/(float)63);
            gradientData[i*4] = col.z();
            gradientData[i*4+1] = col.y();
            gradientData[i*4+2] = col.x();
            gradientData[i*4+3] = col.w();
        }

        mGradientTex.bind();
        mGradientTex.setData(QOpenGLTexture::BGRA, QOpenGLTexture::UInt8, gradientData);
    }
}

void Grid::cleanup()
{
    mHeightTex.destroy();
    mGradientTex.destroy();

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


    mGrid = nullptr;
    mXCount = 0;
    mYCount = 0;
}

void Grid::draw(Camera *camera, Environment*)
{
    Q_ASSERT(camera);

    mVAO.bind();

    QMatrix4x4 mat = camera->view() * matrix();
    mProgram->bind();

    mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
    mProgram->setUniformValue(mMVMatrixLoc, mat);
    mProgram->setUniformValue(mFactorLoc, 1.0f);

    mHeightTex.bind(0);
    mGradientTex.bind(1);

    glDrawElements(GL_TRIANGLE_STRIP, mIndexCount, GL_UNSIGNED_INT, 0);

    if(true)
    {
        mProgram->setUniformValue(mFactorLoc, 0.6f);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glDrawElements(GL_TRIANGLE_STRIP, mIndexCount, GL_UNSIGNED_INT, 0);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    mHeightTex.release(0);
    mGradientTex.release(1);
    mProgram->release();
}

