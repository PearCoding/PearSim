#include "propertyview.h"

#include "properties/propertytable.h"

#include <QHeaderView>
#include <QPainter>
#include <QApplication>
#include <QDebug>

PropertyView::PropertyView(QWidget* parent) :
QTreeWidget(parent),
mProperties(nullptr)
{
	setColumnCount(2);
	setAlternatingRowColors(true);

	QStringList headerList;
	headerList << tr("Property") << tr("Value");
		qDebug() << headerList;
	setHeaderLabels(headerList);

	QPalette p(palette());
	p.setColor(QPalette::AlternateBase, QColor(255, 255, 200));
	p.setColor(QPalette::Base, QColor(255, 255, 160));
	setPalette(p);

	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList{ "Test1", "1" });
	QTreeWidgetItem* child = new QTreeWidgetItem(QStringList{ "Test1_1", "1.1" });
	item->addChild(child);
	child->addChild(new QTreeWidgetItem(QStringList{ "Test1.1.1", "1.1.1" }));

	addTopLevelItem(item);
	addTopLevelItem(new QTreeWidgetItem(QStringList{ "Test2", "2" }));
	addTopLevelItem(new QTreeWidgetItem(QStringList{ "Test3", "3" }));
	addTopLevelItem(new QTreeWidgetItem(QStringList{ "Test4", "4" }));
	
	connect(header(), SIGNAL(sectionDoubleClicked(int)), this, SLOT(resizeColumnToContents(int)));
	header()->setStretchLastSection(true);
}

PropertyView::~PropertyView()
{
}

void PropertyView::setPropertyTable(PropertyTable* table)
{
	if (mProperties)
	{
		reset();
	}

	mProperties = table;

	if (mProperties)
	{
		foreach(IProperty* prop, mProperties->topProperties())
		{

		}
	}
}

PropertyTable* PropertyView::propertyTable() const
{
	return mProperties;
}

void PropertyView::reset()
{
	mProperties = nullptr;
	clear();
}

void PropertyView::drawRow(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QTreeWidgetItem* item = itemFromIndex(index);

	QTreeWidget::drawRow(painter, option, index);

	painter->save();
	painter->setPen(QPen(Qt::gray));
	painter->drawLine(option.rect.x(), option.rect.bottom(), option.rect.right(), option.rect.bottom());
	painter->restore();
}

void PropertyView::drawBranches(QPainter* painter, const QRect & rect, const QModelIndex & index) const
{
	QTreeWidget::drawBranches(painter, rect, index);
	
	painter->save();
	painter->setPen(QPen(Qt::gray));
	painter->drawLine(rect.right(), rect.top(), rect.right(), rect.bottom());
	painter->restore();
}