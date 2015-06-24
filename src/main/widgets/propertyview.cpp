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
	setRootIsDecorated(true);
	setSelectionMode(QAbstractItemView::NoSelection);
	setFocusPolicy(Qt::NoFocus);

	QStringList headerList;
	headerList << tr("Property") << tr("Value");
	setHeaderLabels(headerList);

	QPalette p(palette());
	p.setColor(QPalette::AlternateBase, QColor(255, 255, 200));
	p.setColor(QPalette::Base, QColor(255, 255, 160));
	setPalette(p);
	
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
			QTreeWidgetItem* item = new QTreeWidgetItem(this);
			setupItem(item, prop);
			mMapper.insert(item, prop);

			addChildItems(item, prop);
		}
	}
}

void PropertyView::addChildItems(QTreeWidgetItem* parent, IProperty* property)
{
	foreach(IProperty* prop, property->childs())
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		setupItem(item, prop);
		parent->addChild(item);
		mMapper.insert(item, prop);

		addChildItems(item, prop);
	}
}

QTreeWidgetItem* PropertyView::setupItem(QTreeWidgetItem* item, IProperty* property)
{
	item->setDisabled(!property->isEnabled());
	item->setText(0, property->propertyName());

	item->setStatusTip(0, property->statusTip());
	item->setStatusTip(1, property->statusTip());

	item->setToolTip(0, property->toolTip());
	item->setToolTip(1, property->toolTip());

	item->setWhatsThis(0, property->whatsThis());
	item->setWhatsThis(1, property->whatsThis());
	
	QWidget* editor = property->editorWidget(this);
	if (editor)
	{
		setItemWidget(item, 1, editor);
	}
	else
	{
		if (property->valueText().isEmpty())
		{
			item->setFirstColumnSpanned(true);
			item->setBackgroundColor(0, QColor(180, 180, 180));
			item->setTextColor(0, QColor(255, 255, 255));

			QFont f = item->font(0);
			f.setBold(true);
			item->setFont(0, f);
		}
		else
		{
			item->setText(1, property->valueText());
		}
	}

	return item;
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