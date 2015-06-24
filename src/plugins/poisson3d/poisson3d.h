#pragma once

#include "sims/isimulation.h"
#include "renderer/camera.h"

#include "actors/emptyactor.h"
#include "actors/grid.h"

#include "renderer/material.h"
#include "renderer/environment.h"

class DoubleProperty;
class GroupProperty;
class IntProperty;

class ActorInteractor;
class Poisson3D : public ISimulation
{
	Q_OBJECT
public:
	Poisson3D();
	~Poisson3D();
	
	QString name() const
	{
		return "Poisson 3D";
	}

	int flags() const
	{
		return SF_None;
	}

	IInteractor* interactor();
	PropertyTable* properties();

	virtual void draw(Renderer *);
	virtual void resizeResources(int w, int h);
	virtual void initResources();
	virtual void cleanResources();

private slots:
	void gridStructureChanged();
	void gridValuesChanged();

private:
	void calculate();

	Camera mCamera;

	EmptyActor mRootActor;
	Grid mGrid;

	FloatDataGrid* mDataGrid;
	StandardGradient mGradient;

	Material mMaterial;
	Material mContourMaterial;
	Environment mEnvironment;

	ActorInteractor* mInteractor;
	PropertyTable* mPropertyTable;

	bool mRebuild;

	GroupProperty* mGridGroup;
	IntProperty* mGridSizeProperty;
	DoubleProperty* mGridFactorProperty;
};
