#include <QtTest/QtTest>
#include "data/data.h"

class DataTest : public QObject
{
	Q_OBJECT
private slots:
	void size_data();
	void size();

	void access_data();
	void access();

	void iter_data();
	void iter();

	void split();
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

void DataTest::access_data()
{
	QTest::addColumn<int>("width");
	QTest::addColumn<int>("height");
	QTest::addColumn<int>("depth");
	QTest::addColumn<float>("val1");
	QTest::addColumn<int>("p1_x");
	QTest::addColumn<int>("p1_y");
	QTest::addColumn<int>("p1_z");
	QTest::addColumn<float>("val2");
	QTest::addColumn<int>("p2_x");
	QTest::addColumn<int>("p2_y");
	QTest::addColumn<int>("p2_z");

	QTest::newRow("1D") << 10 << 0 << 0 << 1.0f << 1 << 0 << 0 << 2.0f << 7 << 0 << 0;
	QTest::newRow("2D") << 10 << 5 << 0 << 1.0f << 2 << 2 << 0 << 2.0f << 0 << 3 << 0;
	QTest::newRow("3D") << 5 << 10 << 10 << 1.0f << 3 << 4 << 1 << 2.0f << 4 << 0 << 9;
}

void DataTest::access()
{
	QFETCH(int, width);
	QFETCH(int, height);
	QFETCH(int, depth);
	QFETCH(float, val1);
	QFETCH(int, p1_x);
	QFETCH(int, p1_y);
	QFETCH(int, p1_z);
	QFETCH(float, val2);
	QFETCH(int, p2_x);
	QFETCH(int, p2_y);
	QFETCH(int, p2_z);

	Data<float> data(width, height, depth);

	QVERIFY(data.ptr() != nullptr);
	QCOMPARE(data.at(0), 0.0f);
	QCOMPARE(data[0](), 0.0f);

	Data<float>::element_size_type indx;
	switch(data.dimension())
	{
	case 1:
		indx.clear();
		indx.push_back(p1_x);
		data.set(indx, val1);
		QCOMPARE(data.at(indx), val1);
		
		indx.clear();
		indx.push_back(p2_x);
		data.set(indx, val2);
		QCOMPARE(data.at(indx), val2);
		break;
	case 2:
		indx.clear();
		indx.push_back(p1_x);
		indx.push_back(p1_y);
		data.set(indx, val1);
		QCOMPARE(data.at(indx), val1);

		indx.clear();
		indx.push_back(p2_x);
		indx.push_back(p2_y);
		data.set(indx, val2);
		QCOMPARE(data.at(indx), val2);
		break;
	case 3:
		indx.clear();
		indx.push_back(p1_x);
		indx.push_back(p1_y);
		indx.push_back(p1_z);
		data.set(indx, val1);
		QCOMPARE(data.at(indx), val1);

		indx.clear();
		indx.push_back(p2_x);
		indx.push_back(p2_y);
		indx.push_back(p2_z);
		data.set(indx, val2);
		QCOMPARE(data.at(indx), val2);
		break;
	default:
		QFAIL("Invalid dimension");
	}

	QCOMPARE(data.max(), qMax(0.0f, qMax(val1, val2)));
	QCOMPARE(data.min(), qMin(0.0f, qMin(val1, val2)));
}

void DataTest::iter_data()
{
	QTest::addColumn<int>("width");
	QTest::addColumn<int>("height");
	QTest::addColumn<int>("depth");

	QTest::newRow("1D") << 10 << 0 << 0;
	QTest::newRow("2D") << 10 << 5 << 0;
	QTest::newRow("3D") << 5 << 10 << 10;
}

// Maybe check for access?
void DataTest::iter()
{
	QFETCH(int, width);
	QFETCH(int, height);
	QFETCH(int, depth);

	Data<float> data(width, height, depth);

	Data<float>::size_type count = 0;
	Q_FOREACH(float t, data)
	{
		++count;
	}
	QCOMPARE(count, data.size());

	count = 0;
	for(float t : data)
	{
		++count;
	}
	QCOMPARE(count, data.size());

	count = 0;
	for (Data<float>::const_iterator it = data.begin();
		it != data.end();
		++it)
	{
		++count;
	}
	QCOMPARE(count, data.size());
}

void DataTest::split()
{
	Data<float> data(10, 10);
	data[5][5] = 5;
	QCOMPARE(data.at({ 5, 5 }), 5.0f);

	Data<float> data2 = data.split({ 2, 2 }, { 6, 6 });
	QCOMPARE((int)data2.dimension(), 2);
	QCOMPARE(data2.at({ 3, 3 }), 5.0f);

	Data<float> data3 = data.split({ 5, 2 }, { 5, 6 });
	data3.trim();
	QCOMPARE((int)data3.dimension(), 1);
	QCOMPARE(data3.at({ 3 }), 5.0f);
}

QTEST_APPLESS_MAIN(DataTest)
#include "moc_data_test.cpp"