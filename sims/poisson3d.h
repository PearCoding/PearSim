#ifndef POISSON3D_H
#define POISSON3D_H

#include "isimulation.h"
#include "camera.h"

#include "actors/emptyactor.h"
#include "actors/grid.h"

#include "renderer/material.h"
#include "renderer/environment.h"

class ActorInteractor;
class Poisson3D : public ISimulation
{
public:
    Poisson3D();
    ~Poisson3D();QString name() const
    {
        return "Poisson 3D";
    }

    int flags() const
    {
        return SF_None;
    }

    IInteractor* interactor();

    virtual void draw(Renderer *);
    virtual void resizeResources(int w, int h);
    virtual void initResources();
    virtual void cleanResources();

private:
    Camera mCamera;

    EmptyActor mRootActor;
    Grid mGrid;

    FloatDataGrid mDataGrid;

    Material mMaterial;
    Material mContourMaterial;
    Environment mEnvironment;

    ActorInteractor* mInteractor;
};

#endif // POISSON3D_H
