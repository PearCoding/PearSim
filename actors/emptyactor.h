#ifndef EMPTYACTOR_H
#define EMPTYACTOR_H

#include "iactor.h"

class EmptyActor : public IActor
{
public:
    EmptyActor(IActor* parent = nullptr);
    ~EmptyActor();

    void draw(Camera *camera);
};

#endif // EMPTYACTOR_H
