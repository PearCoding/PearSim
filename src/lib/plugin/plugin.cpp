#include "plugin.h"

#include <QDebug>

Plugin::Plugin(const QString& path, const QString& version) :
mLibrary(path, version), mInitFunction(nullptr), mSimulation(nullptr)
{
}

Plugin::~Plugin()
{
	if (mSimulation)
	{
		delete mSimulation;
	}

	mLibrary.unload();
}

bool Plugin::init()
{
	mLibrary.load();
	if(!mLibrary.isLoaded())
	{
		qCritical() << "Couldn't load library" << mLibrary.fileName() << ":" << mLibrary.errorString();
		return false;
	}

	mInitFunction = (PS_INITPLUGIN_FUNCTION)mLibrary.resolve(PS_INITPLUGIN_NAME);
	if (!mInitFunction)
	{
		qCritical() << "Couldn't load the symbol" << PS_INITPLUGIN_NAME << "from" << mLibrary.fileName() << ":" << mLibrary.errorString();
		return false;
	}

	mSimulation = mInitFunction();
	if (mSimulation != nullptr)
	{
		return true;
	}
	else
	{
		qCritical() << "Couldn't init plugin";
		return false;
	}
}
