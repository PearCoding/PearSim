#ifndef IINTERACTOR_H
#define IINTERACTOR_H

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class IInteractor
{
public:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void keyReleaseEvent(QKeyEvent * event);
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);
    virtual void resizeEvent(QResizeEvent * event);
};

#endif // IINTERACTOR_H
