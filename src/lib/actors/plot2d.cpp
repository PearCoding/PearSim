#include "plot2d.h"

Plot2D::Plot2D(IActor *parent) :
	IActor(parent),
	mGradient(nullptr)
{
}

Plot2D::~Plot2D()
{
	cleanup();
}

void Plot2D::build(const FloatData& plots)
{
	Q_ASSERT(plots.dimension() == 2 && plots.size() > 1);
	Q_ASSERT(plots.elementSize()[0] % 2 == 0);

	const size_t plotcount = plots.elementSize()[0] / 2;
	const size_t datacount = plots.elementSize()[1];
	for (size_t i = 0; i < plotcount; ++i)
	{
		Line2D* line = new Line2D(this);
		if (mGradient)
		{
			QVector4D col = mGradient->value((i + 1) / (float)(plotcount));
			line->setColor(QColor(col.x(), col.y(), col.z(), col.w()));
		}

		FloatData data = plots.split({ i * 2, 0 }, { i * 2 + 1, datacount - 1 });
		line->build(data);

		mPlots.push_back(line);
	}
}

void Plot2D::cleanup()
{
	for (Line2D* line : mPlots)
	{
		line->cleanup();
	}

	qDeleteAll(mPlots);
	mPlots.clear();
}

void Plot2D::draw(Camera *camera, Environment* env)
{
	for (Line2D* line : mPlots)
	{
		line->draw(camera, env);
	}
}

