#pragma once

#include "line2d.h"
#include "utils/gradient.h"

class Material;
class PS_LIB Plot2D : public IActor
{
public:
	Plot2D(IActor* parent = nullptr);
	~Plot2D();

	void build(FloatData* plots);
	void cleanup();

	void draw(Camera *camera, Environment* env);

	inline Gradient<QVector4D>* gradient() const
	{
		return mGradient;
	}

	inline void setGradient(Gradient<QVector4D>* gradient)
	{
		mGradient = gradient;
	}

private:
	QVector<Line2D*> mPlots;
	Gradient<QVector4D>* mGradient;
};
