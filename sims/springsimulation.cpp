#include "springsimulation.h"
#include "interactors/actorinteractor.h"

SpringSimulation::SpringSimulation() :
    ISimulation(),
    mCamera()
{
    mInteractor = new ActorInteractor();
    mInteractor->setActor(&mRootActor);

    mStartBound.setParent(&mRootActor);
    mEndBound.setParent(&mRootActor);

    mStartBound.setPosition(QVector3D(0.5f, 0, 0));
    mEndBound.setPosition(QVector3D(-0.5f, 0, 0));
}

SpringSimulation::~SpringSimulation()
{
    delete mInteractor;
}

IInteractor* SpringSimulation::interactor()
{
    return mInteractor;
}

void SpringSimulation::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mStartBound.draw(&mCamera);
    mEndBound.draw(&mCamera);
}

void SpringSimulation::resizeResources(int w, int h)
{
    ISimulation::resizeResources(w, h);

    QMatrix4x4 proj;
    proj.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
    mCamera.setProjection(proj);
}

void SpringSimulation::initResources()
{
    ISimulation::initResources();

    mCamera.setFocalPoint(QVector3D(0,0,0));
    mCamera.setPosition(QVector3D(0,0,-1));

    mStartBound.build(0.1f, 0.25f, 0.25f);
    mEndBound.build(0.1f, 0.25f, 0.25f);

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);
}

void SpringSimulation::cleanResources()
{
    mStartBound.cleanup();
    mEndBound.cleanup();

    ISimulation::cleanResources();
}
