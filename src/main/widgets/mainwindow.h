#pragma once

#include <QMainWindow>
#include <QOpenGLDebugMessage>

#include "plugin/pluginmanager.h"

namespace Ui {
class MainWindow;
}

class ISimulation;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void openSimulation(ISimulation* sim);

public slots:
	void onMessageLogged(QOpenGLDebugMessage msg);
	void makeScreenshot();

	void showAllDockWidgets();
	void hideAllDockWidgets();

	void showAllToolBars();
	void hideAllToolBars();

	void homepage_sl();
	void about_sl();

protected:
	void closeEvent(QCloseEvent* event);

private:
	void showDockWidgets(bool b);
	void showToolBars(bool b);

	void saveSettings();
	void loadSettings();

	Ui::MainWindow *ui;
	PluginManager mPluginManager;
	ISimulation* mSimulation;

	QOpenGLDebugLogger* mGLLogger;
};
