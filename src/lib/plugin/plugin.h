#pragma once

#include <QString>
#include <QLibrary>

#include "iplugin.h"

class PS_LIB Plugin
{
	friend class PluginManager;
public:
	Plugin(const QString& path = "", const QString& version = "");
	virtual ~Plugin();

	inline void setPath(QString& path)
	{
		mLibrary.setFileName(path);
	}

	inline QString getPath() const
	{
		return mLibrary.fileName();
	}

	inline void setVersion(QString& version)
	{
		mLibrary.setFileNameAndVersion(mLibrary.fileName(), version);
	}

	virtual bool init();

	inline bool isValid() const
	{
		return mValid;
	}

	inline ISimulation* sim() const
	{
		return mSimulation;
	}

protected:
	inline QLibrary* getLibrary()
	{
		return &mLibrary;
	}

private:
	bool mValid;
	QLibrary mLibrary;
	PS_INITPLUGIN_FUNCTION mInitFunction;
	ISimulation* mSimulation;
};
