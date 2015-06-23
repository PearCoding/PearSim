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

	QStringList headerList;
	headerList << tr("Property") << tr("Value");
		qDebug() << headerList;
	setHeaderLabels(headerList);

	QPalette p(palette());
	p.setColor(QPalette::Base, QColor(255,255,200));
	setPalette(p);

	addTopLevelItem(new QTreeWidgetItem(QStringList{ "Test1", "1" }));
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
	QTreeWidget::drawRow(painter, option, index);

	QTreeWidgetItem* item = itemFromIndex(index);

	QColor color(QApplication::style()->styleHint(QStyle::SH_Table_GridLineColor, &option));
	painter->save();
	painter->setPen(QPen(color));
	painter->drawLine(option.rect.x(), option.rect.bottom(), option.rect.right(), option.rect.bottom());
	painter->drawLine(option.rect.x() + indentation(), option.rect.top(), option.rect.x() + indentation(), option.rect.bottom());
	painter->restore();
}