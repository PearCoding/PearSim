#include "actorinteractor.h"
#include "actors/iactor.h"

#include <QKeyEvent>
#include <QMouseEvent>

ActorInteractor::ActorInteractor() :
    mActor(nullptr)
{
}

ActorInteractor::~ActorInteractor()
{
}

void ActorInteractor::setActor(IActor* act)
{
    mActor = act;
}

IActor* ActorInteractor::actor() const
{
    return mActor;
}

void ActorInteractor::keyPressEvent(QKeyEvent * event)
{
    if(!mActor)
    {
        return;
    }

    const float speed = 0.01f;
    const float rotAngle = 10;

    switch(event->key())
    {
    }
}

void ActorInteractor::mouseMoveEvent(QMouseEvent * event)
{
    const float speed = 0.5f;
    if(mActor && event->buttons() & Qt::LeftButton)
    {
        int dx = event->pos().x() - mLastClick.x();
        int dy = event->pos().y() - mLastClick.y();

        //qDebug() << dx << dy;
        mActor->rotate(-dy * speed, dx * speed, 0);
    }
    mLastClick = event->pos();
}

void ActorInteractor::mousePressEvent(QMouseEvent * event)
{
    mLastClick = event->pos();
}
