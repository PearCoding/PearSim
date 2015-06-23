#pragma once

#include "config.h"

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;
class QResizeEvent;
class PS_LIB IInteractor
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

