#pragma once

#include "config.h"
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

#define PS_MAX_LIGHTS (4)

struct PS_LIB_INLINE ShaderPreferences
{
    ShaderPreferences() :
        HasDiffuseTexture(false), HasSpecularTexture(false), HasSmoothTexture(false),
        HasAmbient(false), Lights(0)
    {
    }

    bool HasDiffuseTexture;
    bool HasSpecularTexture;
    bool HasSmoothTexture;
    bool HasAmbient;
    int Lights;
};

class Camera;
class Material;
class Environment;
class PS_LIB Shader// Generic shader for standard stuff
{
public:
    Shader();
    ~Shader();

    inline bool isBuilt() const
    {
        return mProgram != nullptr;
    }

    void build(ShaderPreferences prefs);

    void bind(const QMatrix4x4& mv, Camera *camera, Material* m, Environment* env);
    void release();

private:
    void cleanup();

    ShaderPreferences mPreferences;
    QOpenGLShaderProgram *mProgram;

    int mProjMatrixLoc;
    int mMVMatrixLoc;
    int mDiffuseLoc;
    int mSpecularLoc;
    int mSmoothnessLoc;
    int mAmbientFactorLoc;
    int mAmbientColorLoc;
    int mLightPositionLoc;
    int mLightColorLoc;
};
