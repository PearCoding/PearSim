#include "blocksimulation.h"

BlockSimulation::BlockSimulation() :
    ISimulation(),
    mCamera(), mGrid(this)
{

}

BlockSimulation::~BlockSimulation()
{

}

void BlockSimulation::draw()
{
    mGrid.draw(&mCamera);
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

    mCamera.translate(0,0,-1);

    glClearColor(0,0,0.2f, 1);
    glEnable(GL_DEPTH_TEST);

    mGrid.build(10, 10, 10, 0.05f);
}

void BlockSimulation::cleanResources()
{
    mGrid.cleanup();

    ISimulation::cleanResources();
}
