#include "renderer.h"

#include <QOpenGLFunctions>

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glPolygonOffset(0,1);

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void Renderer::cleanup()
{

}

void Renderer::beginFrame()
{
	//QOpenGLContext::currentContext()->functions()->glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Renderer::endFrame()
{

}

void Renderer::startBackVisibility()
{
	glDisable(GL_CULL_FACE);
}

void Renderer::endBackVisibility()
{
	glEnable(GL_CULL_FACE);
}

void Renderer::resizeResources(int, int)
{

}
