#include "grid.h"
#include "renderer/shader.h"
#include "camera.h"

Grid::Grid(IActor *parent) :
    IActor(parent),
    mShader(nullptr), mMaterial(nullptr), mContourMaterial(nullptr),
    mIndexVBO(QOpenGLBuffer::IndexBuffer),
    mVertexData(nullptr), mIndexData(nullptr),
    mGrid(nullptr), mXCount(0), mYCount(0)
{
}

Grid::~Grid()
{
    cleanup();
}

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
    ShaderPreferences prefs;
    prefs.HasAmbient = false;
    prefs.Lights = 0;
    mShader = new Shader;
    mShader->build(prefs);
}

void Grid::cleanup()
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

    mGrid = nullptr;
    mXCount = 0;
    mYCount = 0;
}

void Grid::draw(Camera *camera, Environment* env)
{
    Q_ASSERT(camera);

    mVAO.bind();

    QMatrix4x4 mat = camera->view() * matrix();
    mShader->bind(mat, camera, mMaterial, env);

    glDrawElements(GL_TRIANGLE_STRIP, mIndexCount, GL_UNSIGNED_INT, 0);

    if(mContourMaterial)
    {
        mShader->bind(mat, camera, mContourMaterial, env);

        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glDrawElements(GL_TRIANGLE_STRIP, mIndexCount, GL_UNSIGNED_INT, 0);

        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    mShader->release();
}

