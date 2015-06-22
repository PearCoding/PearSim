#include "gradient.h"

StandardGradient::StandardGradient()
{
	add(0, QVector4D(255, 0, 0, 255));
	add(0.222f, QVector4D(255, 255, 0, 255));
	add(0.444f, QVector4D(0, 255, 0, 255));
	add(0.666f, QVector4D(0, 255, 255, 255));
	add(0.888f, QVector4D(0, 0, 255, 255));
	add(1, QVector4D(255, 0, 255, 255));
}
