#pragma once

#include <QObject>

#include "plugin.h"

class PS_LIB PluginManager : public QObject
{
	Q_OBJECT
public:
	explicit PluginManager(QObject *parent = 0);
	~PluginManager();

	static inline PluginManager* getInstance()
	{
		return sInstance;
	}

	inline Plugin* loadPlugin(QString path, QString version = "")
	{
		Plugin* plugin = getPlugin(path);

		if(!plugin)
		{
			plugin = new Plugin(path, version);

			if(plugin)
			{
				loadPlugin(plugin);
			}
		}

		return plugin;
	}

	inline void loadPlugin(Plugin* plugin)
	{
		if (!mPlugins.contains(plugin))
		{
			mPlugins.append(plugin);

			if(!plugin->getLibrary()->isLoaded())
			{
				if(!plugin->init())
				{
					qCritical("An error happened while initializing the algorithm plugin");
				}
			}
		}
	}

	inline void removePlugin(Plugin* plugin)
	{
		mPlugins.removeOne(plugin);
	}

	inline bool contains(QString path)
	{
		return getPlugin(path) ? true : false;
	}

	inline Plugin* getPlugin(QString path)
	{
		foreach(Plugin* plugin, mPlugins)
		{
			if(plugin->getPath() == path)
			{
				return plugin;
			}
		}
		return 0;
	}

signals:

public slots:

private:
	static PluginManager* sInstance;
	QList<Plugin*> mPlugins;
};