#include "poisson3d.h"
#include "interactors/actorinteractor.h"
#include "renderer/renderer.h"
#include "plugin/iplugin.h"

#include "properties/propertytable.h"
#include "properties/doubleproperty.h"
#include "properties/groupproperty.h"
#include "properties/intproperty.h"

const int DEFAULT_GRID_SIZE = 100;
const float DEFAULT_GRID_FACTOR = 0.6f;

Poisson3D::Poisson3D() :
	ISimulation(),
	mCamera(),
	mDataGrid(nullptr),
	mGradient(StandardGradient::T_Cool),
	mRebuild(false)
{
	mMaterial.setDiffuse(Qt::white);
	mMaterial.setSpecular(QColor(255, 255, 255));
	mMaterial.setSmoothness(10);

	mContourMaterial.setDiffuse(Qt::red);
	
	mInteractor = new ActorInteractor();
	mInteractor->setActor(&mRootActor);

	mPropertyTable = new PropertyTable;
	mGridGroup = new GroupProperty;
	mGridGroup->setPropertyName(tr("Grid"));

	mGridFactorProperty = new DoubleProperty;
	mGridFactorProperty->setMinValue(0);
	mGridFactorProperty->setMaxValue(1);
	mGridFactorProperty->setDefaultValue(DEFAULT_GRID_FACTOR);
	mGridFactorProperty->setValue(DEFAULT_GRID_FACTOR);
	mGridFactorProperty->setStepSize(0.1);
	mGridFactorProperty->setPropertyName("Line-Factor");
	mGridGroup->addChild(mGridFactorProperty);

	mGridSizeProperty = new IntProperty;
	mGridSizeProperty->setMinValue(2);
	mGridSizeProperty->setDefaultValue(DEFAULT_GRID_SIZE);
	mGridSizeProperty->setValue(DEFAULT_GRID_SIZE);
	mGridSizeProperty->setPropertyName("Size");
	mGridGroup->addChild(mGridSizeProperty);

	mPropertyTable->add(mGridGroup);

	connect(mGridFactorProperty, SIGNAL(valueChanged()), this, SLOT(gridValuesChanged()));
	connect(mGridSizeProperty, SIGNAL(valueChanged()), this, SLOT(gridStructureChanged()));

	mGrid.setParent(&mRootActor);
	mGrid.setGradient(&mGradient);
	mGrid.setGridFactor(DEFAULT_GRID_FACTOR);

	mDataGrid = new FloatDataGrid(DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE);
	calculate();
}

Poisson3D::~Poisson3D()
{
	delete mInteractor;
	delete mGridSizeProperty;
	delete mPropertyTable;
	delete mDataGrid;
}

IInteractor* Poisson3D::interactor()
{
	return mInteractor;
}

PropertyTable* Poisson3D::properties()
{
	return mPropertyTable;
}

void Poisson3D::draw(Renderer* renderer)
{
	if (mRebuild)
	{
		mGrid.cleanup();
		mGrid.build(mDataGrid, 0.05f, 0.4f);
		mRebuild = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->startBackVisibility();
	mGrid.draw(&mCamera, nullptr);
	renderer->endBackVisibility();
}

void Poisson3D::resizeResources(int w, int h)
{
	ISimulation::resizeResources(w, h);

	QMatrix4x4 proj;
	proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
	mCamera.setProjection(proj);
}

void Poisson3D::initResources()
{
	ISimulation::initResources();

	mCamera.setFocalPoint(QVector3D(0,0,0));
	mCamera.setPosition(QVector3D(0,0.25f,-1));

	mGrid.build(mDataGrid, 0.05f, 0.4f);

	glClearColor(0,0,0.4f, 1);
	glEnable(GL_DEPTH_TEST);
}

void Poisson3D::cleanResources()
{
	mGrid.cleanup();

	ISimulation::cleanResources();
}

void Poisson3D::gridStructureChanged()
{
	delete mDataGrid;
	mDataGrid = new FloatDataGrid(mGridSizeProperty->value(), mGridSizeProperty->value());
	calculate();
	
	mRebuild = true;
	emit redrawRequest();
}

void Poisson3D::gridValuesChanged()
{
	mGrid.setGridFactor(mGridFactorProperty->value());
	emit redrawRequest();
}

void Poisson3D::calculate()
{
	for (uint x = 0; x < mDataGrid->width(); ++x)
	{
		float px = 4 * 3.1415*x / (float)mDataGrid->width();
		for (uint y = 0; y < mDataGrid->height(); ++y)
		{
			float py = 4 * 3.1415*y / (float)mDataGrid->height();
			mDataGrid->set(x, y, cos(px) + sin(py));
		}
	}
}

PS_INITPLUGIN
{
	return new Poisson3D;
}