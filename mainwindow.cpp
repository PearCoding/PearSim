#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "sims/blocksimulation.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mGLLogger = new QOpenGLDebugLogger(this);

    connect(mGLLogger, SIGNAL(messageLogged( QOpenGLDebugMessage ) ),
                 this, SLOT( onMessageLogged( QOpenGLDebugMessage ) ),
                 Qt::DirectConnection );

    ui->simulationView->setLogger(mGLLogger);

    mSimulation = new BlockSimulation;
    ui->simulationView->setSimulation(mSimulation);
}

MainWindow::~MainWindow()
{
    delete mGLLogger;

    ui->simulationView->setSimulation(nullptr);
    delete mSimulation;

    delete ui;
}

void MainWindow::onMessageLogged(QOpenGLDebugMessage msg)
{
    qDebug() << msg;
    ui->logWidget->addItem(msg.message());
}
