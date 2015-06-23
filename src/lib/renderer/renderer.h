#pragma once

#include "config.h"

class PS_LIB Renderer
{
public:
    Renderer();
    ~Renderer();

    void init();
    void cleanup();

    void beginFrame();
    void endFrame();

    void startBackVisibility();
    void endBackVisibility();

    void resizeResources(int width, int height);
};

