#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sims/blocksimulation.h"
#include "sims/springsimulation.h"

#include <QFileDialog>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mGLLogger = new QOpenGLDebugLogger(this);

    connect(mGLLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)),
                 this, SLOT(onMessageLogged(QOpenGLDebugMessage)),
                 Qt::DirectConnection);

    ui->simulationView->setLogger(mGLLogger);

    openSimulation( new SpringSimulation );
}

MainWindow::~MainWindow()
{
    delete mGLLogger;

    ui->simulationView->setSimulation(nullptr);
    delete mSimulation;

    delete ui;
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
    qDebug() << msg.severity() << " " << msg.message();

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
        break;
    }

    ui->logWidget->addItem(item);
}
