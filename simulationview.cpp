#include "simulationview.h"

#include <QOpenGLContext>

#include "isimulation.h"
#include "iinteractor.h"

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

void SimulationView::keyPressEvent(QKeyEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->keyPressEvent(event);
        update();
    }
    else
    {
        QOpenGLWidget::keyPressEvent(event);
    }
}

void SimulationView::keyReleaseEvent(QKeyEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->keyReleaseEvent(event);
        update();
    }
    else
    {
        QOpenGLWidget::keyReleaseEvent(event);
    }
}

void SimulationView::mouseDoubleClickEvent(QMouseEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->mouseDoubleClickEvent(event);
        update();
    }
    QOpenGLWidget::mouseDoubleClickEvent(event);
}

void SimulationView::mouseMoveEvent(QMouseEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->mouseMoveEvent(event);
        update();
    }
    QOpenGLWidget::mouseMoveEvent(event);
}

void SimulationView::mousePressEvent(QMouseEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->mousePressEvent(event);
        update();
    }
    QOpenGLWidget::mousePressEvent(event);
}

void SimulationView::mouseReleaseEvent(QMouseEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->mouseReleaseEvent(event);
        update();
    }
    QOpenGLWidget::mouseReleaseEvent(event);
}

void SimulationView::wheelEvent(QWheelEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->wheelEvent(event);
        update();
    }

    QOpenGLWidget::wheelEvent(event);
}

void SimulationView::resizeEvent(QResizeEvent * event)
{
    if(mSimulation && mSimulation->interactor())
    {
        mSimulation->interactor()->resizeEvent(event);
    }

    QOpenGLWidget::resizeEvent(event);
}
