#include "simulationview.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions_3_3_Core>

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
	if (mSimulation)
	{
		connect(mSimulation, SIGNAL(redrawRequest()), this, SLOT(repaint()));
	}
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

QImage SimulationView::makeScreenshot()
{
	const int MULTISAMPLE = 8;

	makeCurrent();

	QOpenGLFunctions_3_3_Core* core = context()->versionFunctions<QOpenGLFunctions_3_3_Core>();
	core->initializeOpenGLFunctions();

	// Setup framebuffer
	GLuint framebufferMS = 0;
	core->glGenFramebuffers(1, &framebufferMS);
	core->glBindFramebuffer(GL_FRAMEBUFFER, framebufferMS);

	// Setup texture
	GLuint renderedTexture;
	core->glGenTextures(1, &renderedTexture);
	core->glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, renderedTexture);
	core->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MULTISAMPLE, GL_RGBA, width(), height(), true);
	
	// Setup depth buffer
	GLuint depthrenderbuffer;
	core->glGenRenderbuffers(1, &depthrenderbuffer);
	core->glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	core->glRenderbufferStorageMultisample(GL_RENDERBUFFER, MULTISAMPLE, GL_DEPTH24_STENCIL8, width(), height());
	core->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	core->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderedTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	core->glDrawBuffers(1, DrawBuffers);

	if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		doneCurrent();
		qDebug() << "Couldn't initialize multisample framebuffer";
		return QImage();
	}

	core->glBindFramebuffer(GL_FRAMEBUFFER, framebufferMS);
	paintGL();

	GLuint framebufferNormal = 0;
	core->glGenFramebuffers(1, &framebufferNormal);
	core->glBindFramebuffer(GL_FRAMEBUFFER, framebufferNormal);

	GLuint colorbufferNormal;
	core->glGenRenderbuffers(1, &colorbufferNormal);
	core->glBindRenderbuffer(GL_RENDERBUFFER, colorbufferNormal);
	core->glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, width(), height());
	core->glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorbufferNormal);

	core->glDrawBuffers(1, DrawBuffers);

	if (core->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		doneCurrent();
		qDebug() << "Couldn't initialize normal framebuffer";
		return QImage();
	}

	core->glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebufferNormal);
	core->glBindFramebuffer(GL_READ_FRAMEBUFFER, framebufferMS);
	//core->glDrawBuffer(GL_COLOR_ATTACHMENT0);
	core->glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

	GLubyte* data = new GLubyte[4 * width()*height()];
	core->glBindFramebuffer(GL_FRAMEBUFFER, framebufferNormal);
	core->glReadBuffer(GL_COLOR_ATTACHMENT0);
	core->glReadPixels(0, 0, width(), height(), GL_RGBA, GL_UNSIGNED_BYTE, data);

	core->glBindFramebuffer(GL_FRAMEBUFFER, 0);

	core->glDeleteFramebuffers(1, &framebufferMS);
	core->glDeleteFramebuffers(1, &framebufferNormal);
	core->glDeleteTextures(1, &renderedTexture);
	core->glDeleteRenderbuffers(1, &depthrenderbuffer);
	doneCurrent();

	QImage image(width(), height(), QImage::Format::Format_RGBA8888);

	for (int y = 0; y < image.height(); ++y)
	{
		memcpy(image.scanLine(y), &data[4 * width()*(image.height() - y - 1)], image.bytesPerLine());
	}

	delete[] data;
	return image;
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
