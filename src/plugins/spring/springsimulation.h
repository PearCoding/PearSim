#pragma once

#include "sims/isimulation.h"
#include "renderer/camera.h"

#include "actors/rectangular.h"
#include "actors/emptyactor.h"
#include "actors/sphere.h"

#include "renderer/material.h"
#include "renderer/environment.h"

class ActorInteractor;
class SpringSimulation : public ISimulation
{
public:
    SpringSimulation();
    ~SpringSimulation();

    QString name() const
    {
        return "Spring-Mass";
    }

    int flags() const
    {
        return SF_Step;
    }

    IInteractor* interactor();

    virtual void draw(Renderer *);
    virtual void resizeResources(int w, int h);
    virtual void initResources();
    virtual void cleanResources();

private:
    Camera mCamera;

    EmptyActor mRootActor;
    Rectangular mStartBound;
    Rectangular mEndBound;
    Sphere mFirstSphere;
    Sphere mSecondSphere;

    Material mSphereMaterial;
    Environment mEnvironment;

    ActorInteractor* mInteractor;
};