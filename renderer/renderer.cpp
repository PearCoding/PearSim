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
    //glDisable(GL_CULL_FACE);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
}

void Renderer::cleanup()
{

}

void Renderer::beginFrame()
{

}

void Renderer::endFrame()
{

}

void Renderer::resizeResources(int width, int height)
{

}
