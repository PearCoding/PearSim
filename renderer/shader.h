#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

#define PS_MAX_LIGHTS (4)

class Camera;
class Material;
class Light;
class Shader
{
public:
    Shader();
    ~Shader();

    struct ShaderPreferences
    {
        bool HasDiffuseTexture;
        bool HasSpecularTexture;
        bool HasSmoothTexture;
        bool HasAmbient;
        int Lights;
    };

    inline bool isBuilt() const
    {
        return mIsBuilt;
    }

    void build(ShaderPreferences prefs);

    void bind(const QMatrix4x4& mv, Camera *camera, Material* m, Light* lights);

private:
    void cleanup();

    bool mIsBuilt;
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

#endif // SHADER_H
