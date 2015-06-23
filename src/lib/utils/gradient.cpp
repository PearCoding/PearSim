#include "gradient.h"

StandardGradient::StandardGradient(Type type)
{
	switch (type)
	{
	default:
	case T_HSV:
		add(0, QVector4D(255, 0, 0, 255));
		add(0.222f, QVector4D(255, 255, 0, 255));
		add(0.444f, QVector4D(0, 255, 0, 255));
		add(0.666f, QVector4D(0, 255, 255, 255));
		add(0.888f, QVector4D(0, 0, 255, 255));
		add(1, QVector4D(255, 0, 255, 255));
		break;
	case T_Hot:
		add(0, QVector4D(60, 0, 0, 255));
		add(0.33f, QVector4D(255, 0, 0, 255));
		add(0.66f, QVector4D(255, 255, 0, 255));
		add(1, QVector4D(255, 255, 255, 255));
		break;
	case T_Cool:
		add(0, QVector4D(0, 0, 127, 255));
		add(0.33f, QVector4D(0, 0, 255, 255));
		add(0.66f, QVector4D(0, 127, 255, 255));
		add(1, QVector4D(0, 255, 255, 255));
		break;
	case T_Gray:
		add(0, QVector4D(0, 0, 0, 255));
		add(1, QVector4D(255, 255, 255, 255));
		break;
	}
}
