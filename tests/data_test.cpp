#include <QtTest/QtTest>
#include "data/data.h"

class DataTest : public QObject
{
	Q_OBJECT
private slots:
	void size_data();
	void size();
};

void DataTest::size_data()
{
	QTest::addColumn<int>("width");
	QTest::addColumn<int>("height");
	QTest::addColumn<int>("depth");
	QTest::addColumn<int>("size");
	QTest::addColumn<int>("dimension");

	QTest::newRow("Initial_1") << 1 << 0 << 0 << 1 << 1;
	QTest::newRow("Initial_2") << 1 << 1 << 0 << 1 << 2;
	QTest::newRow("Initial_3") << 1 << 1 << 1 << 1 << 3;
	QTest::newRow("Initial_4") << 0 << 1 << 0 << 1 << 1;
	QTest::newRow("Initial_5") << 0 << 0 << 1 << 1 << 1;
	QTest::newRow("Initial_6") << 0 << 1 << 1 << 1 << 2;

	QTest::newRow("Random_1") << 32 << 0 << 0 << 32 << 1;
	QTest::newRow("Random_2") << 32 << 12 << 0 << 32 * 12 << 2;
	QTest::newRow("Random_3") << 32 << 17 << 16 << 32 * 17 * 16 << 3;
	QTest::newRow("Random_4") << 0 << 17 << 16 << 17 * 16 << 2;
	QTest::newRow("Random_5") << 0 << 0 << 16 << 16 << 1;
}

void DataTest::size()
{
	QFETCH(int, width);
	QFETCH(int, height);
	QFETCH(int, depth);
	QFETCH(int, size);
	QFETCH(int, dimension);

	Data<float> data(width, height, depth);

	QCOMPARE((int)data.size(), size);
	QCOMPARE((int)data.dimension(), dimension);

	QCOMPARE((int)data.elementSize()[0], width != 0 ? width : (height != 0 ? height : depth));

	if (dimension >= 2)
	{
		QCOMPARE((int)data.elementSize()[1], width != 0 ? (height != 0 ? height : depth) : depth);

		if (dimension == 3)
		{
			QCOMPARE((int)data.elementSize()[2], depth);
		}
	}

	QCOMPARE(data.isSingle(), size == 1);
	QCOMPARE(data.isVector(), size > 1 && dimension == 1);
	QCOMPARE(data.isMultidimensional(), dimension > 1);
}

QTEST_APPLESS_MAIN(DataTest)
#include "moc_data_test.cpp"