#include "simulationview.h"

#include <QOpenGLContext>

#include "isimulation.h"

SimulationView::SimulationView(QWidget* parent) :
    QOpenGLWidget(parent),
    mSimulation(nullptr), mLogger(nullptr)
{

}

SimulationView::~SimulationView()
{
    cleanup();
}

void SimulationView::setSimulation(ISimulation* sim)
{
    mSimulation = sim;
}

void SimulationView::setLogger(QOpenGLDebugLogger* log)
{
    mLogger = log;
}

void SimulationView::cleanup()
{
    if(mSimulation)
    {
        mSimulation->cleanResources();
        mSimulation = nullptr;
    }
}

void SimulationView::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SimulationView::cleanup);

    if (mLogger && mLogger->initialize())
    {
        mLogger->startLogging( QOpenGLDebugLogger::SynchronousLogging );
        mLogger->enableMessages();
    }

    if(mSimulation)
    {
        mSimulation->initResources();
    }
}

void SimulationView::paintGL()
{
    if(mSimulation)
    {
        mSimulation->draw();
    }
}

void SimulationView::resizeGL(int width, int height)
{
    if(mSimulation)
    {
        mSimulation->resizeResources(width, height);
    }
}
