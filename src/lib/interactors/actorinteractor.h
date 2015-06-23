#pragma once

#include "iinteractor.h"
#include <QPoint>

class IActor;
class PS_LIB ActorInteractor : public IInteractor
{
public:
    ActorInteractor();
    ~ActorInteractor();

    void setActor(IActor *act);
    IActor* actor() const;

    virtual void keyPressEvent(QKeyEvent * event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mousePressEvent(QMouseEvent * event);
private:
    IActor* mActor;

    QPoint mLastClick;
};
