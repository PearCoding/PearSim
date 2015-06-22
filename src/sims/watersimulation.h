#ifndef WATERSIMULATION_H
#define WATERSIMULATION_H

#include "isimulation.h"
#include "renderer/camera.h"

#include "actors/emptyactor.h"
#include "actors/mesh.h"

#include "renderer/material.h"
#include "renderer/environment.h"

class ActorInteractor;
class WaterSimulation : public ISimulation
{
public:
    WaterSimulation();
    ~WaterSimulation();

    QString name() const
    {
        return "Water";
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
    Mesh mMeshActor;

    Material mMaterial;
    Environment mEnvironment;

    ActorInteractor* mInteractor;
};

#endif // WATERSIMULATION_H
