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
    uint ID;//Will be set later
};

inline bool operator == (const UniqueVertex& f1, const UniqueVertex& f2)
{
    return f1.VIn == f2.VIn && f1.TIn == f2.TIn && f1.NIn == f2.NIn;
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

    bool ok;
    UniqueVertex v;
    v.VIn = Vert.toInt(&ok)-1;
    v.TIn = Tex.toInt(&ok)-1;
    v.NIn = Norm.toInt(&ok)-1;
    v.ID = 0;

    if(!ok)
    {
        qWarning() << "Invalid face part definition";
    }

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

    int lnr = 1;
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

            bool ok;

            QVector3D vert;
            vert.setX(commands.front().toFloat(&ok));
            commands.pop_front();
            vert.setY(commands.front().toFloat(&ok));
            commands.pop_front();

            if(commands.empty())
            {
                vert.setZ(0);
            }
            else
            {
                vert.setZ(commands.front().toFloat(&ok));
            }

            if(!ok)
            {
                qWarning() << "["<< lnr << "] Invalid floating point values";
            }

            vertices.append(vert);
        }
        else if(commands.first() == "vt")//Texture
        {
            commands.pop_front();

            bool ok;

            QVector2D tex;
            tex.setX(commands.front().toFloat(&ok));
            commands.pop_front();
            tex.setY(commands.front().toFloat(&ok));
            commands.pop_front();

            if(!ok)
            {
                qWarning() << "["<< lnr << "] Invalid floating point values";
            }

            texels.append(tex);
        }
        else if(commands.first() == "vn")//Normal
        {
            commands.pop_front();

            bool ok;
            QVector3D norm;
            norm.setX(commands.front().toFloat(&ok));
            commands.pop_front();
            norm.setY(commands.front().toFloat(&ok));
            commands.pop_front();

            if(commands.empty())
            {
                norm.setZ(0);
            }
            else
            {
                norm.setZ(commands.front().toFloat(&ok));
            }

            if(!ok)
            {
                qWarning() << "["<< lnr << "] Invalid floating point values";
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

                int it1 = uniqueVertices.indexOf(uv1);
                int fi1 = uniqueVertices.size();
                if(it1 != -1)
                {
                    fi1 = it1;
                }
                else
                {
                    uniqueVertices.append(uv1);
                }

                int it2 = uniqueVertices.indexOf(uv2);
                int fi2 = uniqueVertices.size();
                if(it2 != -1)
                {
                    fi2 = it2;
                }
                else
                {
                    uniqueVertices.append(uv2);
                }

                int it3 = uniqueVertices.indexOf(uv3);
                int fi3 = uniqueVertices.size();
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
                    int fi4 = uniqueVertices.size();
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
                    f2.UIn[0] = fi4;
                    f2.UIn[1] = fi2;
                    f2.UIn[2] = fi3;
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

        ++lnr;
    }

    // Data conversion [Simple triangles]
    float* vertexData  = new float[uniqueVertices.size()*(3+3+2)];
    uint* indexData = new uint[faces.size()*3];

    int indexCounter = 0;
    int fitr = 0;
    foreach(Face f, faces)
    {
        for(int i = 0; i < 3; ++i)
        {
            UniqueVertex& v = uniqueVertices[f.UIn[i]];

            int in = indexCounter;
            if(v.ID != 0)
            {
                in = v.ID-1;
            }
            else
            {
                size_t index = indexCounter*(3+3+2);
                QVector3D vert = vertices.at(v.VIn);

                QVector2D tex;
                if(texels.size() > v.TIn)
                {
                    tex = texels.at(v.TIn);
                }

                QVector3D norm;
                if(normals.size() > v.NIn)
                {
                    norm = normals.at(v.NIn);
                }

                vertexData[index] = vert.x()*mScale; vertexData[index+1] = vert.y()*mScale; vertexData[index+2] = vert.z()*mScale;
                vertexData[index+3] = norm.x(); vertexData[index+4] = norm.y(); vertexData[index+5] = norm.z();
                vertexData[index+6] = tex.x(); vertexData[index+7] = tex.y();
                v.ID = in+1;
                indexCounter++;
            }
            indexData[fitr*3+i] = in;
        }

        fitr++;
    }

    // Load to mesh
    mesh->load(vertexData, uniqueVertices.size(), indexData, faces.size()*3);
    qDebug() << uniqueVertices.size() << " " << faces.size();
}
