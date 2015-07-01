#include <QtTest/QtTest>
#include "utils/gradient.h"

class GradientTest : public QObject
{
	Q_OBJECT
private slots:
	void linear();
	void multilinear();
};

void GradientTest::linear()
{
	LinearGradient<float> gradient;

	gradient.setStartValue(0);
	gradient.setEndValue(1);

	QCOMPARE(gradient.startValue(), 0.0f);
	QCOMPARE(gradient.endValue(), 1.0f);
	QCOMPARE(gradient.value(0.5f), 0.5f);
}

void GradientTest::multilinear()
{
	MultiLinearGradient<float> gradient;

	gradient.add(0,0);
	gradient.add(0.5f, 1);
	gradient.add(1, 0);

	QCOMPARE(gradient.value(0), 0.0f);
	QCOMPARE(gradient.value(0.25f), 0.5f);
	QCOMPARE(gradient.value(0.5f), 1.0f);
	QCOMPARE(gradient.value(0.75f), 0.5f);
	QCOMPARE(gradient.value(1), 0.0f);
}

QTEST_APPLESS_MAIN(GradientTest)
#include "moc_gradient_test.cpp"