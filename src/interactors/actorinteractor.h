#ifndef ACTORINTERACTOR_H
#define ACTORINTERACTOR_H

#include "iinteractor.h"
#include <QPoint>

class IActor;
class ActorInteractor : public IInteractor
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

#endif // ACTORINTERACTOR_H
