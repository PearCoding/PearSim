#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <QString>

class Mesh;
class MeshLoader
{
public:
    virtual void load(const QString& file, Mesh* mesh) = 0;
};

#endif // MESHLOADER_H
