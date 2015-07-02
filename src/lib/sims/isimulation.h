#pragma once

#include "config.h"
#include <QObject>

enum SimulationFlags
{
	SF_None = 0x0,
	SF_Step = 0x1
};

class IInteractor;
class PropertyTable;
class Renderer;
class PS_LIB ISimulation : public QObject
{
	Q_OBJECT
public:
	ISimulation();
	virtual ~ISimulation();

	virtual QString name() const = 0;
	virtual int flags() const = 0;

	virtual IInteractor* interactor();
	virtual PropertyTable* properties();

	virtual void draw(Renderer* renderer);
	virtual void resizeResources(int w, int h);
	virtual void initResources();
	virtual void cleanResources();

signals:
	void redrawRequest();
};