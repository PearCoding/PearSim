#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLDebugMessage>

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

protected:
    void closeEvent(QCloseEvent* event);

private:
    void showDockWidgets(bool b);
    void showToolBars(bool b);

    void saveSettings();
    void loadSettings();

    Ui::MainWindow *ui;
    ISimulation* mSimulation;

    QOpenGLDebugLogger* mGLLogger;
};

#endif // MAINWINDOW_H
