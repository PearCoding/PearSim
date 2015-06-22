#pragma once

#include <QOpenGLWidget>
#include <QOpenGLDebugLogger>

#include "renderer/renderer.h"

class ISimulation;
class SimulationView : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit SimulationView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
    virtual ~SimulationView();

    void setSimulation(ISimulation* sim);
    void setLogger(QOpenGLDebugLogger* log);
public slots:
    void cleanup();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    void mouseDoubleClickEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent * event) Q_DECL_OVERRIDE;
    void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent * event) Q_DECL_OVERRIDE;

private:
    Renderer mRenderer;
    ISimulation* mSimulation;
    QOpenGLDebugLogger* mLogger;
};
