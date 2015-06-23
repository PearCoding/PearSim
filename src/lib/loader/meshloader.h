#pragma once

#include <QString>
#include "config.h"

class Mesh;
class PS_LIB_INLINE MeshLoader
{
public:
    virtual void load(const QString& file, Mesh* mesh) = 0;
};

