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

void ISimulation::draw(Renderer*)
{

}

void ISimulation::resizeResources(int, int)
{

}

void ISimulation::initResources()
{
    initializeOpenGLFunctions();
}

void ISimulation::cleanResources()
{

}
