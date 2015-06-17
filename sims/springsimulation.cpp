#include "springsimulation.h"
#include "interactors/actorinteractor.h"

#include "renderer/light.h"

#include "loader/wavefrontloader.h"

SpringSimulation::SpringSimulation() :
    ISimulation(),
    mCamera()
{
    mSphereMaterial.setDiffuse(Qt::blue);
    mSphereMaterial.setSpecular(QColor(1, 0,0));
    mSphereMaterial.setSmoothness(10);

    //mEnvironment.setAmbientFactor(1.0f);
    Light* light = new Light(&mRootActor);
    light->setPosition(QVector3D(2,2,2));
    mEnvironment.addLight(light);

    mInteractor = new ActorInteractor();
    mInteractor->setActor(&mRootActor);

    mStartBound.setParent(&mRootActor);
    mEndBound.setParent(&mRootActor);
    mFirstSphere.setParent(&mRootActor);
    mSecondSphere.setParent(&mRootActor);
    mMeshActor.setParent(&mRootActor);

    mStartBound.setPosition(QVector3D(0.5f, 0, 0));
    mEndBound.setPosition(QVector3D(-0.5f, 0, 0));
    mFirstSphere.setPosition(QVector3D(0.2f, 0, 0));
    mSecondSphere.setPosition(QVector3D(-0.2f, 0, 0));

    mFirstSphere.setMaterial(&mSphereMaterial);
    mSecondSphere.setMaterial(&mSphereMaterial);
    mStartBound.setMaterial(&mSphereMaterial);
    mEndBound.setMaterial(&mSphereMaterial);
    mMeshActor.setMaterial(&mSphereMaterial);
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
    mStartBound.draw(&mCamera, &mEnvironment);
    mEndBound.draw(&mCamera, &mEnvironment);
    mFirstSphere.draw(&mCamera, &mEnvironment);
    mSecondSphere.draw(&mCamera, &mEnvironment);
    mMeshActor.draw(&mCamera, &mEnvironment);
}

void SpringSimulation::resizeResources(int w, int h)
{
    ISimulation::resizeResources(w, h);

    QMatrix4x4 proj;
    proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
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

    WavefrontLoader loader;
    loader.setScale(0.025f);
    loader.load("../PearSimulation/content/torus.obj", &mMeshActor);

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);
}

void SpringSimulation::cleanResources()
{
    mStartBound.cleanup();
    mEndBound.cleanup();
    mFirstSphere.cleanup();
    mSecondSphere.cleanup();
    mMeshActor.cleanup();

    ISimulation::cleanResources();
}
