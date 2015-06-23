#include "simulationview.h"

#include <QOpenGLContext>

#include "sims/isimulation.h"
#include "interactors/iinteractor.h"

SimulationView::SimulationView(QWidget* parent, Qt::WindowFlags f) :
QOpenGLWidget(parent, f),
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
	makeCurrent();

	if (mSimulation)
	{
		mSimulation->cleanResources();
		mSimulation = nullptr;
	}

	mRenderer.cleanup();

	if (mLogger)
	{
		mLogger->stopLogging();
	}
	doneCurrent();
}

void SimulationView::initializeGL()
{
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &SimulationView::cleanup, Qt::DirectConnection);

	if (mLogger && mLogger->initialize())
	{
		mLogger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
		mLogger->enableMessages();
	}

	mRenderer.init();

	if (mSimulation)
	{
		mSimulation->initResources();
	}
}

void SimulationView::paintGL()
{
	mRenderer.beginFrame();
	if (mSimulation)
	{
		mSimulation->draw(&mRenderer);
	}
	mRenderer.endFrame();
}

void SimulationView::resizeGL(int width, int height)
{
	mRenderer.resizeResources(width, height);

	if (mSimulation)
	{
		mSimulation->resizeResources(width, height);
	}
}

void SimulationView::keyPressEvent(QKeyEvent * event)
{
	if (mSimulation && mSimulation->interactor())
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
	if (mSimulation && mSimulation->interactor())
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
	if (mSimulation && mSimulation->interactor())
	{
		mSimulation->interactor()->mouseDoubleClickEvent(event);
		update();
	}
	QOpenGLWidget::mouseDoubleClickEvent(event);
}

void SimulationView::mouseMoveEvent(QMouseEvent * event)
{
	if (mSimulation && mSimulation->interactor())
	{
		mSimulation->interactor()->mouseMoveEvent(event);
		update();
	}
	QOpenGLWidget::mouseMoveEvent(event);
}

void SimulationView::mousePressEvent(QMouseEvent * event)
{
	if (mSimulation && mSimulation->interactor())
	{
		mSimulation->interactor()->mousePressEvent(event);
		update();
	}
	QOpenGLWidget::mousePressEvent(event);
}

void SimulationView::mouseReleaseEvent(QMouseEvent * event)
{
	if (mSimulation && mSimulation->interactor())
	{
		mSimulation->interactor()->mouseReleaseEvent(event);
		update();
	}
	QOpenGLWidget::mouseReleaseEvent(event);
}

void SimulationView::wheelEvent(QWheelEvent * event)
{
	if (mSimulation && mSimulation->interactor())
	{
		mSimulation->interactor()->wheelEvent(event);
		update();
	}

	QOpenGLWidget::wheelEvent(event);
}

void SimulationView::resizeEvent(QResizeEvent * event)
{
	if (mSimulation && mSimulation->interactor())
	{
		mSimulation->interactor()->resizeEvent(event);
	}

	QOpenGLWidget::resizeEvent(event);
}
