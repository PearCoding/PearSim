#ifndef BLOCKSIMULATION_H
#define BLOCKSIMULATION_H

#include "isimulation.h"
#include "renderer/camera.h"
#include "actors/gridbox.h"

class CameraInteractor;
class BlockSimulation : public ISimulation
{
    Q_OBJECT
public:
    BlockSimulation();
    virtual ~BlockSimulation();

    QString name() const
    {
        return "Block";
    }

    int flags() const
    {
        return SF_Step;
    }

    IInteractor* interactor();

    virtual void draw(Renderer* renderer);
    virtual void resizeResources(int w, int h);
    virtual void initResources();
    virtual void cleanResources();

private:
    Camera mCamera;

    GridBox mGrid;
    CameraInteractor* mInteractor;
};

#endif // BLOCKSIMULATION_H