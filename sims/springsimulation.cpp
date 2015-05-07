#include "springsimulation.h"
#include "interactors/actorinteractor.h"

SpringSimulation::SpringSimulation() :
    ISimulation(),
    mCamera()
{
    mSphereMaterial.setDiffuse(Qt::blue);

    mInteractor = new ActorInteractor();
    mInteractor->setActor(&mRootActor);

    mStartBound.setParent(&mRootActor);
    mEndBound.setParent(&mRootActor);
    mFirstSphere.setParent(&mRootActor);
    mSecondSphere.setParent(&mRootActor);

    mStartBound.setPosition(QVector3D(0.5f, 0, 0));
    mEndBound.setPosition(QVector3D(-0.5f, 0, 0));
    mFirstSphere.setPosition(QVector3D(0.2f, 0, 0));
    mSecondSphere.setPosition(QVector3D(-0.2f, 0, 0));

    mFirstSphere.setMaterial(&mSphereMaterial);
    mSecondSphere.setMaterial(&mSphereMaterial);
}

SpringSimulation::~SpringSimulation()
{
    delete mInteractor;
}

IInteractor* SpringSimulation::interactor()
{
    return mInteractor;
}

void SpringSimulation::draw(Renderer*)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mStartBound.draw(&mCamera, nullptr);
    mEndBound.draw(&mCamera, nullptr);
    mFirstSphere.draw(&mCamera, nullptr);
    mSecondSphere.draw(&mCamera, nullptr);
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
    mFirstSphere.build(0.1f, 16, 16);
    mSecondSphere.build(0.1f, 16, 16);

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);
}

void SpringSimulation::cleanResources()
{
    mStartBound.cleanup();
    mEndBound.cleanup();
    mFirstSphere.cleanup();
    mSecondSphere.cleanup();

    ISimulation::cleanResources();
}
