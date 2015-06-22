#include "watersimulation.h"
#include "interactors/actorinteractor.h"

#include "renderer/light.h"

#include "loader/wavefrontloader.h"

WaterSimulation::WaterSimulation() :
    ISimulation(),
    mCamera()
{
    mMaterial.setDiffuse(QColor(255, 230, 20));
    mMaterial.setSpecular(QColor(255,255,255));
    mMaterial.setSmoothness(100);

    //mEnvironment.setAmbientFactor(1.0f);
    Light* light = new Light(&mRootActor);
    light->setPosition(QVector3D(2,2,2));
    mEnvironment.addLight(light);

    mInteractor = new ActorInteractor();
    mInteractor->setActor(&mRootActor);

    mMeshActor.setParent(&mRootActor);
    mMeshActor.setMaterial(&mMaterial);
}

WaterSimulation::~WaterSimulation()
{
    delete mInteractor;
}

IInteractor* WaterSimulation::interactor()
{
    return mInteractor;
}

void WaterSimulation::draw(Renderer*)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mMeshActor.draw(&mCamera, &mEnvironment);
}

void WaterSimulation::resizeResources(int w, int h)
{
    ISimulation::resizeResources(w, h);

    QMatrix4x4 proj;
    proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
    mCamera.setProjection(proj);
}

void WaterSimulation::initResources()
{
    ISimulation::initResources();

    mCamera.setFocalPoint(QVector3D(0,0,0));
    mCamera.setPosition(QVector3D(0,0.5f,-1));

    WavefrontLoader loader;
    loader.setScale(0.1f);
    loader.load("../PearSimulation/content/water_pot.obj", &mMeshActor);

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);
}

void WaterSimulation::cleanResources()
{
    mMeshActor.cleanup();

    ISimulation::cleanResources();
}
