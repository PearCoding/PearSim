#ifndef RENDERER_H
#define RENDERER_H


class Renderer
{
public:
    Renderer();
    ~Renderer();

    void init();
    void cleanup();

    void beginFrame();
    void endFrame();

    void resizeResources(int width, int height);
};

#endif // RENDERER_H
