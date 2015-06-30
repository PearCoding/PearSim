#include "line2d.h"
#include "renderer/shader.h"
#include "renderer/camera.h"

Line2D::Line2D(IActor *parent) :
	IActor(parent),
	mProgram(nullptr), mProjMatrixLoc(0), mMVMatrixLoc(0),
	mColorLoc(0),
	mIndexVBO(QOpenGLBuffer::IndexBuffer),
	mVertexData(nullptr), mIndexData(nullptr),
	mData(nullptr), mColor(Qt::black)
{
}

Line2D::~Line2D()
{
	cleanup();
}

static const char *vertexShaderSourceCore =
		"#version 150\n"
		"in vec4 vertex;\n"
		"in mediump vec3 normal;\n"
		"in lowp vec2 uv;\n"
		"out mediump vec3 N;\n"
		"out lowp vec2 Tex;\n"
		"uniform mat4 projMatrix;\n"
		"uniform mat4 mvMatrix;\n"
		"void main() {\n"
		"   N = (mvMatrix*vec4(normal,0)).xyz;\n"
		"   Tex = uv;\n"
		"   gl_Position = projMatrix * mvMatrix * vertex;\n"
		"}\n";

static const char *fragmentShaderSourceCore =
		"#version 150\n"
		"in mediump vec3 N;\n"
		"in lowp vec2 Tex;\n"
		"out highp vec4 fragColor;\n"
		"uniform vec4 color;\n"
		"void main() {\n"
		"   fragColor = color;\n"
		"}\n";

void Line2D::build(FloatData* data)
{
	Q_ASSERT(data && data->dimension() == 2 && data->size() > 1);
	Q_ASSERT(data->elementSize()[0] == 2);
	
	if(mData)
	{
		cleanup();
	}

	mData = data;

	mVertexCount = data->elementSize()[1];
	mVertexData = new GLfloat[mVertexCount*8];
	
	const size_t slide = data->elementSize()[1];
	for(int i = 0; i < mVertexCount; ++i)
	{
		mVertexData[i * 8 + 0] = data->at(i * 2);
		mVertexData[i * 8 + 1] = data->at(i * 2 + 1);
		mVertexData[i * 8 + 2] = 0;

		mVertexData[i * 8 + 3] = 0; mVertexData[i * 8 + 4] = 0; mVertexData[i * 8 + 5] = 1;//TODO
		mVertexData[i * 8 + 6] = 0; mVertexData[i * 8 + 7] = 0;
	}

	// OpenGL code
	mVAO.create();
	mVAO.bind();

	mVBO.create();
	mVBO.bind();
	mVBO.allocate(mVertexData, mVertexCount * sizeof(GLfloat) * 8);

	mIndexVBO.create();
	mIndexVBO.bind();
	mIndexVBO.allocate(mIndexData, mIndexCount * sizeof(GLuint));

	qDebug() << "Vertices " << mVertexCount << " Indices " << mIndexCount;
	mProgram = new QOpenGLShaderProgram;
	mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
	mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceCore);
	mProgram->bindAttributeLocation("vertex", 0);
	mProgram->bindAttributeLocation("normal", 1);
	mProgram->bindAttributeLocation("uv", 2);
	mProgram->link();

	mProgram->bind();
	mProjMatrixLoc = mProgram->uniformLocation("projMatrix");
	mMVMatrixLoc = mProgram->uniformLocation("mvMatrix");
	mColorLoc = mProgram->uniformLocation("color");

	mProgram->enableAttributeArray("vertex");
	mProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 8*sizeof(GLfloat));

	mProgram->enableAttributeArray("normal");
	mProgram->setAttributeBuffer("normal", GL_FLOAT, 3*sizeof(GLfloat), 3, 8*sizeof(GLfloat));

	mProgram->enableAttributeArray("uv");
	mProgram->setAttributeBuffer("uv", GL_FLOAT, 6*sizeof(GLfloat), 2, 8*sizeof(GLfloat));
}

void Line2D::cleanup()
{
	mIndexVBO.destroy();
	mVBO.destroy();
	mVAO.destroy();

	if(mProgram)
	{
		delete mProgram;
		mProgram = nullptr;
	}

	if(mVertexData)
	{
		delete [] mVertexData;
		mVertexData = nullptr;
	}

	if(mIndexData)
	{
		delete [] mIndexData;
		mIndexData = nullptr;
	}

	mData = nullptr;
}

void Line2D::draw(Camera *camera, Environment*)
{
	Q_ASSERT(camera);

	if (!mData)
	{
		return;
	}

	mVAO.bind();

	QMatrix4x4 mat = camera->view() * matrix();
	mProgram->bind();

	mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
	mProgram->setUniformValue(mMVMatrixLoc, mat);
	mProgram->setUniformValue(mColorLoc, mColor);
	
	glDrawArrays(GL_LINE_STRIP, 0, mVertexCount);

	mProgram->release();
}

