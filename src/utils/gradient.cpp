#include "gradient.h"

StandardGradient::StandardGradient()
{
    add(0, QVector4D(255,0,0,255));
    add(0.5f, QVector4D(0,255,0,255));
    add(1, QVector4D(0,0,255,255));
}
