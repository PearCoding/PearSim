#include "shader.h"

#include "camera.h"
#include "material.h"
#include "light.h"
#include "environment.h"

Shader::Shader() :
    mProgram(nullptr)
{

}

Shader::~Shader()
{
    cleanup();
}

static const char *vertexShaderSourceCore =
        "#version 150\n"
        "in vec4 vertex;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "}\n";

void Shader::build(ShaderPreferences prefs)
{
    cleanup();

    QString fragmentShader;
    fragmentShader += "#version 150\n"
                      "in mediump vec3 normal;\n"
                      "in lowp vec2 uv;\n"
                      "out highp vec4 fragColor;\n";

    if(prefs.HasDiffuseTexture)
    {
        fragmentShader += "uniform sampler2D diffuse;\n";
    }
    else
    {
        fragmentShader += "uniform vec4 diffuse;\n";
    }

    if(prefs.HasSpecularTexture)
    {
        fragmentShader += "uniform sampler2D specular;\n";
    }
    else
    {
        fragmentShader += "uniform vec4 specular;\n";
    }

    if(prefs.HasSmoothTexture)
    {
        fragmentShader += "uniform sampler2D smoothness;\n";
    }
    else
    {
        fragmentShader += "uniform float smoothness;\n";
    }

    if(prefs.HasAmbient)
    {
        fragmentShader += "uniform float ambientFactor;\n"
                            "uniform vec4 ambientColor;\n";
    }

    if(prefs.Lights > 0)
    {
        fragmentShader += "uniform vec3 lightPositions[" + QString::number(prefs.Lights) + "];\n";
        fragmentShader += "uniform vec4 lightColors[" + QString::number(prefs.Lights) + "];\n";
    }

    fragmentShader += "void main() {\n";
    if(prefs.Lights <= 0) //No lights
    {
        if(prefs.HasDiffuseTexture)
        {
            fragmentShader += "   vec4 color = texture(diffuse, uv);\n";
        }
        else
        {
            fragmentShader += "   vec4 color = diffuse;\n";
        }

        if(prefs.HasAmbient)
        {
            fragmentShader += "   fragColor = (color * ambientColor) * ambientFactor;\n";
        }
        else
        {
            fragmentShader += "   fragColor = color;\n";
        }
    }
    else
    {
        if(prefs.HasDiffuseTexture)
        {
            fragmentShader += "   vec4 color = texture(diffuse, uv);\n";
        }
        else
        {
            fragmentShader += "   vec4 color = diffuse;\n";
        }

        if(prefs.HasSpecularTexture)
        {
            fragmentShader += "   vec4 spec = texture(specular, uv);\n";
        }
        else
        {
            fragmentShader += "   vec4 spec = specular;\n";
        }

        if(prefs.HasSmoothTexture)
        {
            fragmentShader += "   float smooth = texture(smoothness, uv).r;\n";
        }
        else
        {
            fragmentShader += "   float smooth = smoothness;\n";
        }

        fragmentShader += "   vec4 output;\n";

        if(prefs.HasAmbient)
        {
            fragmentShader += "   output = (color*ambientColor)*ambientFactor;\n";
        }

        for(int i = 0; i < prefs.Lights; ++i)
        {
            fragmentShader += "   float n" + QString::number(i)
                                + " = dot(normal, lightPositions[" + QString::number(i) +"]);\n"
                              "   output += diffuse*lightColors[" + QString::number(i) +"]*n;\n";
        }
    }

    fragmentShader += "}\n";

    qDebug() << fragmentShader;

    mProgram = new QOpenGLShaderProgram;
    mProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceCore);
    mProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
    mProgram->bindAttributeLocation("vertex", 0);
    mProgram->bindAttributeLocation("normal", 1);
    mProgram->bindAttributeLocation("uv", 2);
    mProgram->link();

    mProgram->bind();
    mProjMatrixLoc = mProgram->uniformLocation("projMatrix");
    mMVMatrixLoc = mProgram->uniformLocation("mvMatrix");

    mDiffuseLoc = mProgram->uniformLocation("diffuse");
    mSpecularLoc = mProgram->uniformLocation("specular");
    mSmoothnessLoc = mProgram->uniformLocation("smoothness");

    if(prefs.HasAmbient)
    {
        mAmbientFactorLoc = mProgram->uniformLocation("ambientFactor");
        mAmbientColorLoc = mProgram->uniformLocation("ambientColor");
    }

    if(prefs.Lights > 0)
    {
        mLightPositionLoc = mProgram->uniformLocation("lightPositions");
        mLightColorLoc = mProgram->uniformLocation("lightColors");
    }

    mProgram->enableAttributeArray("vertex");
    mProgram->setAttributeBuffer("vertex", GL_FLOAT, 0, 3, 8*sizeof(float));

    mProgram->enableAttributeArray("normal");
    mProgram->setAttributeBuffer("normal", GL_FLOAT, 3*sizeof(float), 3, 8*sizeof(float));

    mProgram->enableAttributeArray("uv");
    mProgram->setAttributeBuffer("uv", GL_FLOAT, 6*sizeof(float), 2, 8*sizeof(float));
}

void Shader::bind(const QMatrix4x4& mv, Camera* camera, Material* m, Environment *env)
{
    mProgram->bind();

    mProgram->setUniformValue(mProjMatrixLoc, camera->projection());
    mProgram->setUniformValue(mMVMatrixLoc, mv);

    mProgram->setUniformValue(mDiffuseLoc, m->diffuse());//TODO Texture
    mProgram->setUniformValue(mSpecularLoc, m->specular());
    mProgram->setUniformValue(mSmoothnessLoc, m->smoothness());

    if(env && mPreferences.HasAmbient)
    {
        mProgram->setUniformValue(mAmbientFactorLoc, env->ambientFactor());
        mProgram->setUniformValue(mAmbientColorLoc, env->ambientColor());
    }

    if(env && mPreferences.Lights > 0)
    {
        Q_ASSERT(mPreferences.Lights <= PS_MAX_LIGHTS);

        QVector4D colors[PS_MAX_LIGHTS];
        QVector3D positions[PS_MAX_LIGHTS];

        for(int i = 0; i < mPreferences.Lights && i < PS_MAX_LIGHTS; ++i)
        {
            QColor col = env->light(i)->color();

            colors[i] = QVector4D(col.redF(), col.greenF(), col.blueF(), col.alphaF());
            positions[i] = env->light(i)->position();
        }

        mProgram->setUniformValueArray(mLightColorLoc, colors, mPreferences.Lights);
        mProgram->setUniformValueArray(mLightPositionLoc, positions, mPreferences.Lights);
    }
}

void Shader::release()
{
    mProgram->release();
}

void Shader::cleanup()
{
    if(mProgram)
    {
        delete mProgram;
    }
}
