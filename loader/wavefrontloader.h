#ifndef WAVEFRONTLOADER_H
#define WAVEFRONTLOADER_H

#include "meshloader.h"

class WavefrontLoader : public MeshLoader
{
public:
    WavefrontLoader();
    ~WavefrontLoader();

    void load(const QString& file, Mesh* mesh);
};

#endif // WAVEFRONTLOADER_H
