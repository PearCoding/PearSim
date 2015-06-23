#pragma once

#include "iactor.h"

class PS_LIB EmptyActor : public IActor
{
public:
    EmptyActor(IActor* parent = nullptr);
    virtual ~EmptyActor();

    void draw(Camera *camera, Environment* env);
};

