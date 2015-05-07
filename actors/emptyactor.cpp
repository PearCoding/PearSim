#include "emptyactor.h"

EmptyActor::EmptyActor(IActor *parent) :
    IActor(parent)
{

}

EmptyActor::~EmptyActor()
{

}

void EmptyActor::draw(Camera*, Environment*)
{
}
