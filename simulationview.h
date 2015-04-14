#ifndef SIMULATIONVIEW_H
#define SIMULATIONVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLDebugLogger>

class ISimulation;
class SimulationView : public QOpenGLWidget
{
    Q_OBJECT

public:
    SimulationView(QWidget* parent = nullptr);
    ~SimulationView();

    void setSimulation(ISimulation* sim);
    void setLogger(QOpenGLDebugLogger* log);
public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

private:
    ISimulation* mSimulation;
    QOpenGLDebugLogger* mLogger;
};

#endif // SIMULATIONVIEW_H
