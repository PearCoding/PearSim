#include "blocksimulation.h"
#include "interactors/camerainteractor.h"

BlockSimulation::BlockSimulation() :
    ISimulation(),
    mCamera(), mGrid()
{
    mInteractor = new CameraInteractor(&mCamera);
}

BlockSimulation::~BlockSimulation()
{
    delete mInteractor;
}

IInteractor* BlockSimulation::interactor()
{
    return mInteractor;
}

void BlockSimulation::draw(Renderer*)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mGrid.draw(&mCamera, nullptr);
}

void BlockSimulation::resizeResources(int w, int h)
{
    ISimulation::resizeResources(w, h);

    QMatrix4x4 proj;
    proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
    mCamera.setProjection(proj);
}

void BlockSimulation::initResources()
{
    ISimulation::initResources();

    mCamera.setFocalPoint(QVector3D(0,0,0));
    mCamera.setPosition(QVector3D(0,0,-1));

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);

    mGrid.build(10, 10, 10, 0.05f);
}

void BlockSimulation::cleanResources()
{
    mGrid.cleanup();

    ISimulation::cleanResources();
}
