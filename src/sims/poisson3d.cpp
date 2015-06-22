#include "poisson3d.h"
#include "interactors/actorinteractor.h"

#include "renderer/renderer.h"

Poisson3D::Poisson3D() :
    ISimulation(),
    mCamera(),
    mDataGrid(100, 100)
{
    mMaterial.setDiffuse(Qt::white);
    mMaterial.setSpecular(QColor(255, 255, 255));
    mMaterial.setSmoothness(10);

    mContourMaterial.setDiffuse(Qt::red);

    //mEnvironment.setAmbientFactor(1);

    mInteractor = new ActorInteractor();
    mInteractor->setActor(&mRootActor);

    mGrid.setParent(&mRootActor);
    mGrid.setGradient(&mGradient);

    for(uint x = 0; x < mDataGrid.width(); ++x)
    {
        float px = 4*3.1415*x/(float)mDataGrid.width();
        for(uint y = 0; y < mDataGrid.height(); ++y)
        {
            float py = 4*3.1415*y/(float)mDataGrid.height();
            mDataGrid.set(x,y, cos(px)+sin(py));
        }
    }
}

Poisson3D::~Poisson3D()
{
    delete mInteractor;
}

IInteractor* Poisson3D::interactor()
{
    return mInteractor;
}

void Poisson3D::draw(Renderer* renderer)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer->startBackVisibility();
    mGrid.draw(&mCamera, nullptr);
    renderer->endBackVisibility();
}

void Poisson3D::resizeResources(int w, int h)
{
    ISimulation::resizeResources(w, h);

    QMatrix4x4 proj;
    proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
    mCamera.setProjection(proj);
}

void Poisson3D::initResources()
{
    ISimulation::initResources();

    mCamera.setFocalPoint(QVector3D(0,0,0));
    mCamera.setPosition(QVector3D(0,0.25f,-1));

    mGrid.build(&mDataGrid, 0.05f, 0.4f);

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);
}

void Poisson3D::cleanResources()
{
    mGrid.cleanup();

    ISimulation::cleanResources();
}

