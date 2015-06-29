#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "iactor.h"
#include "data/data.h"
#include "utils/gradient.h"

class Material;
class PS_LIB Grid : public IActor
{
public:
	Grid(IActor* parent = nullptr);
	~Grid();

	void build(FloatData* grid, float spacing = 1.0f, float hSpaceing = 1.0f);
	void build(size_t xc, size_t yc, float spacing, float hSpaceing);
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

	inline float gridFactor() const
	{
		return mGridFactor;
	}

	inline void setGridFactor(float f)
	{
		mGridFactor = f;
	}
private:
	QOpenGLVertexArrayObject mVAO;
	QOpenGLBuffer mVBO;
	QOpenGLBuffer mIndexVBO;
	QOpenGLShaderProgram *mProgram;

	int mProjMatrixLoc;
	int mMVMatrixLoc;
	int mHeightLoc;
	int mGradientLoc;
	int mFactorLoc;
	int mHeightBoundaryLoc;

	int mVertexCount;
	int mIndexCount;
	GLfloat* mVertexData;
	GLuint* mIndexData;

	FloatData* mGrid;
	size_t mXCount;
	size_t mYCount;

	QOpenGLTexture mHeightTex;
	QOpenGLTexture mGradientTex;

	float mGridFactor;
	Gradient<QVector4D>* mGradient;
};
