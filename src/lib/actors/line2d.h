#pragma once

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QColor>

#include "iactor.h"
#include "data/data.h"

class PS_LIB Line2D : public IActor
{
public:
	Line2D(IActor* parent = nullptr);
	~Line2D();

	void build(FloatData* pos);
	void cleanup();

	void draw(Camera *camera, Environment* env);

	inline QColor color() const
	{
		return mColor;
	}

	inline void setColor(const QColor& c)
	{
		mColor = c;
	}

private:
	QOpenGLVertexArrayObject mVAO;
	QOpenGLBuffer mVBO;
	QOpenGLBuffer mIndexVBO;
	QOpenGLShaderProgram *mProgram;

	int mProjMatrixLoc;
	int mMVMatrixLoc;
	int mColorLoc;

	size_t mVertexCount;
	size_t mIndexCount;
	GLfloat* mVertexData;
	GLuint* mIndexData;

	FloatData* mData;
	QColor mColor;
};
