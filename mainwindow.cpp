#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sims/blocksimulation.h"
#include "sims/springsimulation.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuDockwidgets->addAction(ui->logDW->toggleViewAction());
    ui->menuDockwidgets->addAction(ui->propertiesDW->toggleViewAction());
    ui->menuToolbars->addAction(ui->mainToolBar->toggleViewAction());

    mGLLogger = new QOpenGLDebugLogger(this);

    connect(mGLLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
                 this, SLOT(onMessageLogged(QOpenGLDebugMessage)),
                 Qt::DirectConnection);

    ui->simulationView->setLogger(mGLLogger);

    openSimulation( new SpringSimulation );

    loadSettings();
}

MainWindow::~MainWindow()
{
    delete mGLLogger;

    ui->simulationView->setSimulation(nullptr);
    delete mSimulation;

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
        this->setWindowTitle("PearSimulation | " + sim->name());
    }
    else
    {
        this->setWindowTitle("PearSimulation");
    }

    mSimulation = sim;
    ui->simulationView->setSimulation(mSimulation);
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
