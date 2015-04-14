#ifndef BLOCKSIMULATION_H
#define BLOCKSIMULATION_H

#include "isimulation.h"
#include "camera.h"
#include "utils/gridbox.h"

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

    virtual void draw();
    virtual void resizeResources(int w, int h);
    virtual void initResources();
    virtual void cleanResources();

private:
    Camera mCamera;

    GridBox mGrid;
};

#endif // BLOCKSIMULATION_H
