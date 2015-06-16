#ifndef WAVEFRONTLOADER_H
#define WAVEFRONTLOADER_H

#include "meshloader.h"

class WavefrontLoader : public MeshLoader
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

#endif // WAVEFRONTLOADER_H
