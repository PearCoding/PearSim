#ifndef ISIMULATION_H
#define ISIMULATION_H

#include <QOpenGLFunctions>

enum SimulationFlags
{
    SF_None = 0x0,
    SF_Step = 0x1
};

class ISimulation : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    ISimulation();
    virtual ~ISimulation();

    virtual QString name() const = 0;
    virtual int flags() const = 0;

    virtual void draw();
    virtual void resizeResources(int w, int h);
    virtual void initResources();
    virtual void cleanResources();
};

#endif // ISIMULATION_H
