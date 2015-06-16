#include "wavefrontloader.h"

#include <QFile>
#include <QStringList>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QHash>

#include "actors/mesh.h"

// Slow implementation!
WavefrontLoader::WavefrontLoader() :
    mScale(1)
{
}

WavefrontLoader::~WavefrontLoader()
{
}

struct UniqueVertex
{
    int VIn;
    int TIn;
    int NIn;
};

inline bool operator == (const UniqueVertex& f1, const UniqueVertex& f2)
{
    return f1.VIn == f2.VIn && f1.TIn == f2.TIn && f1.NIn == f2.NIn;
}

inline uint qHash(const UniqueVertex &key)
{
    return (key.VIn + key.NIn*1000000 + key.TIn*1000000*100000);
}

struct Face//Indices to unique vertices
{
    int UIn[3];
};

UniqueVertex unpackFaceVertex(const QString& str)
{
    QString Vert;
    QString Tex;
    QString Norm;

    int part = 0;

    foreach(QChar c, str)
    {
        if(c == '/')
        {
            part++;

            if(part > 2)
            {
                break;
            }
        }
        else
        {
            switch(part)
            {
            case 0:
                Vert += c;
                break;
            case 1:
                Tex += c;
                break;
            case 2:
                Norm += c;
                break;
            }
        }
    }

    if(Tex.isEmpty())
    {
        Tex = Vert;
    }

    if(Norm.isEmpty())
    {
        Norm = Vert;
    }

    UniqueVertex v;
    v.VIn = Vert.toInt()-1;
    v.TIn = Tex.toInt()-1;
    v.NIn = Norm.toInt()-1;

    return v;
}

void WavefrontLoader::load(const QString& file, Mesh* mesh)
{
    Q_ASSERT(mesh);

    QFile input(file);

    if(!input.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Couldn't open file " << file;
        return;
    }

    QVector<QVector3D> vertices;
    QVector<QVector2D> texels;
    QVector<QVector3D> normals;
    QVector<UniqueVertex> uniqueVertices;
    QVector<Face> faces;

    while (!input.atEnd())
    {
        QByteArray line = input.readLine().trimmed();
        if(line.isEmpty())
        {
            continue;
        }

        // Parse line
        QStringList commands;
        QString tmp;
        foreach(QChar c, line)
        {
            if(c.isSpace())
            {
                if(!tmp.isEmpty())
                {
                    commands.append(tmp);
                    tmp = "";
                }
            }
            else if(c == '#')//Comment
            {
                break;
            }
            else
            {
                tmp += c;
            }
        }

        if(!tmp.isEmpty())
        {
            commands.append(tmp);
        }

        if(commands.empty())
        {
            continue;
        }

        // Parse Command
        if(commands.first() == "v")//Vertex
        {
            commands.pop_front();

            QVector3D vert;
            vert.setX(commands.front().toFloat());
            commands.pop_front();
            vert.setY(commands.front().toFloat());
            commands.pop_front();

            if(commands.empty())
            {
                vert.setZ(0);
            }
            else
            {
                vert.setZ(commands.front().toFloat());
            }

            vertices.append(vert);
        }
        else if(commands.first() == "vt")//Texture
        {
            commands.pop_front();

            QVector2D tex;
            tex.setX(commands.front().toFloat());
            commands.pop_front();
            tex.setY(commands.front().toFloat());
            commands.pop_front();

            texels.append(tex);
        }
        else if(commands.first() == "vn")//Normal
        {
            commands.pop_front();

            QVector3D norm;
            norm.setX(commands.front().toFloat());
            commands.pop_front();
            norm.setY(commands.front().toFloat());
            commands.pop_front();

            if(commands.empty())
            {
                norm.setZ(0);
            }
            else
            {
                norm.setZ(commands.front().toFloat());
            }

            normals.append(norm);
        }
        else if(commands.first() == "f")//Face
        {
            commands.pop_front();

            if(commands.size() >= 3)//Triangles or quads
            {
                UniqueVertex uv1 = unpackFaceVertex(commands.first());
                commands.pop_front();
                UniqueVertex uv2 = unpackFaceVertex(commands.first());
                commands.pop_front();
                UniqueVertex uv3 = unpackFaceVertex(commands.first());
                commands.pop_front();

                int fi1 = uniqueVertices.size();
                int fi2 = fi1+1;
                int fi3 = fi1+2;

                int it1 = uniqueVertices.indexOf(uv1);
                int it2 = uniqueVertices.indexOf(uv2);
                int it3 = uniqueVertices.indexOf(uv3);

                if(it1 != -1)
                {
                    fi1 = it1;
                }
                else
                {
                    uniqueVertices.append(uv1);
                }

                if(it2 != -1)
                {
                    fi2 = it2;
                }
                else
                {
                    uniqueVertices.append(uv2);
                }

                if(it3 != -1)
                {
                    fi3 = it3;
                }
                else
                {
                    uniqueVertices.append(uv3);
                }

                if(!commands.isEmpty())//Quad
                {
                    UniqueVertex uv4 = unpackFaceVertex(commands.first());
                    int fi4 = fi1+3;
                    int it4 = uniqueVertices.indexOf(uv4);

                    if(it4 != -1)
                    {
                        fi4 = it4;
                    }
                    else
                    {
                        uniqueVertices.append(uv4);
                    }

                    Face f1;
                    f1.UIn[0] = fi1;
                    f1.UIn[1] = fi2;
                    f1.UIn[2] = fi4;
                    faces.append(f1);

                    Face f2;
                    f2.UIn[0] = fi2;
                    f2.UIn[1] = fi3;
                    f2.UIn[2] = fi4;
                    faces.append(f2);
                }
                else//Triangle
                {
                    Face f1;
                    f1.UIn[0] = fi1;
                    f1.UIn[1] = fi2;
                    f1.UIn[2] = fi3;
                    faces.append(f1);
                }
            }
        }
        //Everything else is ignored
    }

    // Data conversion [Simple triangles]
    float* vertexData  = new float[uniqueVertices.size()*(3+3+2)];
    uint* indexData = new uint[faces.size()*3];

    QHash<UniqueVertex, uint> indexMap;
    GLint indexCounter = 0;
    int fitr = 0;
    foreach(Face f, faces)
    {
        for(int i = 0; i < 3; ++i)
        {
            UniqueVertex v = uniqueVertices.at(f.UIn[i]);

            int in = indexCounter;
            if(indexMap.contains(v))
            {
                in = indexMap[v];
            }
            else
            {
                size_t index = indexCounter*(3+3+2);
                QVector3D vert = vertices.at(v.VIn);
                QVector2D tex = texels.at(v.TIn);
                QVector3D norm = normals.at(v.NIn);

                vertexData[index] = vert.x()*mScale; vertexData[index+1] = vert.y()*mScale; vertexData[index+2] = vert.z()*mScale;
                vertexData[index+3] = norm.x(); vertexData[index+4] = norm.y(); vertexData[index+5] = norm.z();
                vertexData[index+6] = tex.x(); vertexData[index+7] = tex.y();
                indexMap[v] = in;
                indexCounter++;
            }
            indexData[fitr*3+i] = in;
        }

        fitr++;
    }

    // Load to mesh
    mesh->load(vertexData, uniqueVertices.size(), indexData, faces.size());
}
