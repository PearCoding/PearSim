#ifndef EMPTYACTOR_H
#define EMPTYACTOR_H

#include "iactor.h"

class EmptyActor : public IActor
{
public:
    EmptyActor(IActor* parent = nullptr);
    virtual ~EmptyActor();

    void draw(Camera *camera, Environment* env);
};

#endif // EMPTYACTOR_H
