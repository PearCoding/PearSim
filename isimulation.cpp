#include "isimulation.h"

ISimulation::ISimulation() :
    QOpenGLFunctions()
{

}

ISimulation::~ISimulation()
{

}

IInteractor* ISimulation::interactor()
{
    return nullptr;
}

void ISimulation::draw()
{

}

void ISimulation::resizeResources(int w, int h)
{

}

void ISimulation::initResources()
{
    initializeOpenGLFunctions();
}

void ISimulation::cleanResources()
{

}
