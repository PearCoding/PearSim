#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sims/isimulation.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>
#include <QMessageBox>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	mSimulation(nullptr)
{
	setWindowTitle(PS_APP_TITLE);
	ui->setupUi(this);

	ui->menuDockwidgets->addAction(ui->logDW->toggleViewAction());
	ui->menuDockwidgets->addAction(ui->propertiesDW->toggleViewAction());
	ui->menuToolbars->addAction(ui->mainToolBar->toggleViewAction());

	connect(ui->actionHomepage, SIGNAL(triggered()), this, SLOT(homepage_sl()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about_sl()));
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), QApplication::instance(), SLOT(aboutQt()));

	mGLLogger = new QOpenGLDebugLogger(this);

	connect(mGLLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
				 this, SLOT(onMessageLogged(QOpenGLDebugMessage)),
				 Qt::DirectConnection);

	ui->simulationView->setLogger(mGLLogger);

	QString plName = "ps_poisson3d_plugin";
#ifdef PS_DEBUG
	plName += "_d";
#endif
	Plugin* plugin = mPluginManager.loadPlugin(plName);

	if (plugin && plugin->isValid())
	{
		openSimulation(plugin->sim());
	}

	loadSettings();
}

MainWindow::~MainWindow()
{
	//delete mGLLogger;
	if(mSimulation)
	{
		mSimulation->cleanResources();
		ui->simulationView->setSimulation(nullptr);
	}

	delete ui;
}

void MainWindow::closeEvent(QCloseEvent*)
{
	saveSettings();
}

void MainWindow::openSimulation(ISimulation* sim)
{
	if (sim)
	{
		setWindowTitle(PS_APP_TITLE " | " + sim->name());
	}
	else
	{
		setWindowTitle(PS_APP_TITLE);
	}

	mSimulation = sim;
	ui->simulationView->setSimulation(mSimulation);

	if (sim)
	{
		ui->propertyView->setPropertyTable(mSimulation->properties());
		ui->propertyView->expandToDepth(1);
	}
	else
	{
		ui->propertyView->reset();
	}
}

void MainWindow::makeScreenshot()
{
	QImage image = ui->simulationView->grabFramebuffer();

	QString path = QFileDialog::getSaveFileName(this, tr("Save screenshot to.."),
												QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
												tr("Images (*.png *.xpm *.jpg)"));

	if (!path.isEmpty())
	{
		image.save(path);
	}
}

void MainWindow::onMessageLogged(QOpenGLDebugMessage msg)
{
	QListWidgetItem* item = new QListWidgetItem(msg.message());
	switch(msg.severity())
	{
	case QOpenGLDebugMessage::HighSeverity:
		item->setBackgroundColor(Qt::red);
		break;
	case QOpenGLDebugMessage::MediumSeverity:
		item->setBackgroundColor(QColor(127, 127, 0));
		break;
	case QOpenGLDebugMessage::LowSeverity:
		item->setBackgroundColor(Qt::yellow);
		return;//Ignore
		break;
	}

	qDebug() << msg.severity() << " " << msg.message();
	ui->logWidget->addItem(item);
}

void MainWindow::showAllDockWidgets()
{
	showDockWidgets(true);
}

void MainWindow::hideAllDockWidgets()
{
	showDockWidgets(false);
}

void MainWindow::showAllToolBars()
{
	showToolBars(true);
}

void MainWindow::hideAllToolBars()
{
	showToolBars(false);
}

void MainWindow::showDockWidgets(bool b)
{
	ui->logDW->setVisible(b);
	ui->propertiesDW->setVisible(b);
}

void MainWindow::showToolBars(bool b)
{
	ui->mainToolBar->setVisible(b);
}

void MainWindow::saveSettings()
{
	QSettings settings;
	settings.beginGroup("Main");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState());
	settings.endGroup();
}

void MainWindow::loadSettings()
{
	QSettings settings;
	settings.beginGroup("Main");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray());
	settings.endGroup();
}

void MainWindow::homepage_sl()
{
	QDesktopServices::openUrl(QUrl("http://pearcoding.eu/"));
}

void MainWindow::about_sl()
{
	QMessageBox::about(this, tr("About PearSim"),
		tr("<h3>About PearSim</h3>"
		"<p>Version: " PS_VERSION_STRING "</p>"
		"<p>Simulation and benchmarking framework for 2D/3D, numerical, ODE, PDE, fluids and other experiments.</p>"
		"<p>Website: <a href='http://pearcoding.eu/'>pearcoding.eu</a><br />"
		"GitHub: <a href='https://github.com/PearCoding/PearSim'>PearSim</a></p>"));
}