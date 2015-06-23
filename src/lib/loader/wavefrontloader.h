#pragma once

#include "meshloader.h"

class PS_LIB WavefrontLoader : public MeshLoader
{
public:
    WavefrontLoader();
    ~WavefrontLoader();

    inline float scale() const
    {
        return mScale;
    }

    inline void setScale(float f)
    {
        mScale = f;
    }

    void load(const QString& file, Mesh* mesh);

private:
    float mScale;
};