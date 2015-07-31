#include "poisson3d.h"
#include "interactors/actorinteractor.h"
#include "renderer/renderer.h"
#include "plugin/iplugin.h"

#include "properties/propertytable.h"
#include "properties/colorproperty.h"
#include "properties/doubleproperty.h"
#include "properties/groupproperty.h"
#include "properties/intproperty.h"
#include "properties/selectionproperty.h"

#include "properties/textproperty.h"

#include "math/interpolation/newton.h"

const int DEFAULT_GRID_SIZE = 100;
const float DEFAULT_GRID_FACTOR = 0.6f;
const float DEFAULT_GRID_SPACING = 0.05f;
const float DEFAULT_GRID_HSPACING = 0.4f;

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
	mGridFactorProperty->setDecimals(2);
	mGridFactorProperty->setPropertyName(tr("Line Factor"));
	mGridGroup->addChild(mGridFactorProperty);

	mGridSizeProperty = new IntProperty;
	mGridSizeProperty->setMinValue(2);
	mGridSizeProperty->setDefaultValue(DEFAULT_GRID_SIZE);
	mGridSizeProperty->setValue(DEFAULT_GRID_SIZE);
	mGridSizeProperty->setPropertyName(tr("Size"));
	mGridGroup->addChild(mGridSizeProperty);

	mGridSpacingProperty = new DoubleProperty;
	mGridSpacingProperty->setMinValue(0.00001f);
	mGridSpacingProperty->setDefaultValue(DEFAULT_GRID_SPACING);
	mGridSpacingProperty->setValue(DEFAULT_GRID_SPACING);
	mGridSpacingProperty->setStepSize(0.01);
	mGridSpacingProperty->setPropertyName(tr("Spacing"));
	mGridGroup->addChild(mGridSpacingProperty);

	mGridHSpacingProperty = new DoubleProperty;
	mGridHSpacingProperty->setMinValue(0.01f);
	mGridHSpacingProperty->setDefaultValue(DEFAULT_GRID_HSPACING);
	mGridHSpacingProperty->setValue(DEFAULT_GRID_HSPACING);
	mGridHSpacingProperty->setStepSize(0.1);
	mGridHSpacingProperty->setPropertyName(tr("Height Spacing"));
	mGridGroup->addChild(mGridHSpacingProperty);
	mPropertyTable->add(mGridGroup);

	mLookGroup = new GroupProperty;
	mLookGroup->setPropertyName(tr("Look"));

	mColorMapProperty = new SelectionProperty;
	mColorMapProperty->setPropertyName(tr("Colormap"));
	mColorMapProperty->addItem(tr("HSV"));
	mColorMapProperty->addItem(tr("Hot"));
	mColorMapProperty->addItem(tr("Cool"));
	mColorMapProperty->addItem(tr("Gray"));
	mColorMapProperty->setIndex(2);
	mLookGroup->addChild(mColorMapProperty);

	mBackgroundColorProperty = new ColorProperty;
	mBackgroundColorProperty->setColor(Qt::black);
	mBackgroundColorProperty->setDefaultColor(Qt::black);
	mBackgroundColorProperty->setPropertyName(tr("Background"));
	mLookGroup->addChild(mBackgroundColorProperty);

	mTextProperty = new TextProperty;
	mTextProperty->setPropertyName("Test");
	mLookGroup->addChild(mTextProperty);

	mPropertyTable->add(mLookGroup);

	connect(mGridFactorProperty, SIGNAL(valueChanged()), this, SLOT(gridValuesChanged()));
	connect(mGridSpacingProperty, SIGNAL(valueChanged()), this, SLOT(gridStructureChanged()));
	connect(mGridHSpacingProperty, SIGNAL(valueChanged()), this, SLOT(gridStructureChanged()));
	connect(mGridSizeProperty, SIGNAL(valueChanged()), this, SLOT(gridStructureChanged()));
	connect(mColorMapProperty, SIGNAL(valueChanged()), this, SLOT(gridStructureChanged()));
	connect(mBackgroundColorProperty, SIGNAL(valueChanged()), this, SLOT(gridStructureChanged()));

	mGrid.setParent(&mRootActor);
	mGrid.setGradient(&mGradient);
	mGrid.setGridFactor(DEFAULT_GRID_FACTOR);

	mPlot.setGradient(&mGradient);
	mPlot.setPosition(QVector3D(100, 400, 0));
	mPlot.setScale(500);

	mDataGrid = new FloatData(DEFAULT_GRID_SIZE, DEFAULT_GRID_SIZE);
	calculate();
}

Poisson3D::~Poisson3D()
{
	delete mInteractor;

	delete mGridSpacingProperty;
	delete mGridHSpacingProperty;
	delete mGridFactorProperty;
	delete mGridSizeProperty;
	delete mPropertyTable;

	delete mColorMapProperty;
	delete mBackgroundColorProperty;
	delete mLookGroup;

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
		mGrid.build(mDataGrid, mGridSpacingProperty->value(), mGridHSpacingProperty->value());

		QColor bg = mBackgroundColorProperty->color();
		glClearColor(bg.redF(), bg.greenF(), bg.blueF(), bg.alphaF());
		mRebuild = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderer->startBackVisibility();
	mGrid.draw(&mCamera, nullptr);
	renderer->endBackVisibility();

	renderer->start2D();
	mPlot.draw(&mHUDCamera, nullptr);
	renderer->end2D();
}

void Poisson3D::resizeResources(int w, int h)
{
	ISimulation::resizeResources(w, h);

	QMatrix4x4 proj;
	proj.perspective(60.0f, GLfloat(w) / h, 0.01f, 100.0f);
	mCamera.setProjection(proj);

	QMatrix4x4 orth;
	orth.ortho(0, w, h, 0, -1, 1);
	mHUDCamera.setProjection(orth);
}

void Poisson3D::initResources()
{
	ISimulation::initResources();

	mCamera.setFocalPoint(QVector3D(0,0,0));
	mCamera.setPosition(QVector3D(0, 0.25f, -1));

	mHUDCamera.setFocalPoint(QVector3D(0, 0, -1));
	mHUDCamera.setPosition(QVector3D(0, 0, 0));

	mGrid.build(mDataGrid, DEFAULT_GRID_SPACING, DEFAULT_GRID_HSPACING);

	const size_t QUALITY = 32;
	FloatData data(4, QUALITY);
	for (FloatData::size_type i = 0; i < QUALITY; ++i)
	{
		data.set({ 0, i }, i / (float)QUALITY);
		data.set({ 1, i }, 0.5f*sin(2 * 3.14156f*i / QUALITY));
		data.set({ 2, i }, i / (float)QUALITY);
		data.set({ 3, i }, 0.5f*cos(2 * 3.14156f*i / QUALITY));
	}
	mPlot.build(data);

	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
}

void Poisson3D::cleanResources()
{
	mGrid.cleanup();
	mPlot.cleanup();

	ISimulation::cleanResources();
}

void Poisson3D::gridStructureChanged()
{
	delete mDataGrid;
	mDataGrid = new FloatData(mGridSizeProperty->value(), mGridSizeProperty->value());
	calculate();

	switch (mColorMapProperty->index())
	{
	case 0:
		mGradient = StandardGradient(StandardGradient::T_HSV);
		mGrid.setGradient(&mGradient);
		break;
	case 1:
		mGradient = StandardGradient(StandardGradient::T_Hot);
		mGrid.setGradient(&mGradient);
		break;
	case 2:
		mGradient = StandardGradient(StandardGradient::T_Cool);
		mGrid.setGradient(&mGradient);
		break;
	case 3:
		mGradient = StandardGradient(StandardGradient::T_Gray);
		mGrid.setGradient(&mGradient);
		break;
	}
	
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
	const FloatData::size_type w = mDataGrid->elementSize()[0];
	const FloatData::size_type h = mDataGrid->elementSize()[1];

	for (FloatData::size_type x = 0; x < w; ++x)
	{
		float px = 4 * 3.1415*x / (float)w;
		for (FloatData::size_type y = 0; y < h; ++y)
		{
			float py = 4 * 3.1415*y / (float)h;
			mDataGrid->set(x + y*w, cos(px) + sin(py));
		}
	}
}

PS_INITPLUGIN
{
	return new Poisson3D;
}