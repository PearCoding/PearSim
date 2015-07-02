#include "isimulation.h"

ISimulation::ISimulation() :
	QObject()
{

}

ISimulation::~ISimulation()
{

}

IInteractor* ISimulation::interactor()
{
	return nullptr;
}

PropertyTable* ISimulation::properties()
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
}

void ISimulation::cleanResources()
{

}
