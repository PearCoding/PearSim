#include "propertytable.h"

PropertyTable::PropertyTable()
{
	connect(&mPropertyChangedMapper, SIGNAL(mapped(QObject*)), this, SLOT(propertyWasChanged(QObject*)));
	connect(&mPropertyStructureChangedMapper, SIGNAL(mapped(QObject*)), this, SLOT(propertyStructureWasChanged(QObject*)));
	connect(&mValueChangedMapper, SIGNAL(mapped(QObject*)), this, SLOT(valueWasChanged(QObject*)));
}

PropertyTable::~PropertyTable()
{

}

void PropertyTable::add(IProperty* property)
{
	if (mProperties.contains(property))
	{
		return;
	}

	mProperties.append(property);

	connect(property, SIGNAL(propertyDestroyed(IProperty*)), this, SLOT(propertyWasDestroyed(IProperty*)));
	connect(property, SIGNAL(propertyChanged()), &mPropertyChangedMapper, SLOT(map()));
	connect(property, SIGNAL(propertyStructureChanged()), &mPropertyStructureChangedMapper, SLOT(map()));
	connect(property, SIGNAL(valueChanged()), &mValueChangedMapper, SLOT(map()));

	foreach(IProperty* child, property->childs())
	{
		add(child);
	}
}

void PropertyTable::remove(IProperty* property)
{
	if (!mProperties.contains(property))
	{
		return;
	}

	mProperties.removeOne(property);

	disconnect(property, SIGNAL(propertyDestroyed(IProperty*)), this, SLOT(propertyWasDestroyed(IProperty*)));
	disconnect(property, SIGNAL(propertyChanged()), &mPropertyChangedMapper, SLOT(map()));
	disconnect(property, SIGNAL(propertyStructureChanged()), &mPropertyStructureChangedMapper, SLOT(map()));
	disconnect(property, SIGNAL(valueChanged()), &mValueChangedMapper, SLOT(map()));

	foreach(IProperty* child, property->childs())
	{
		remove(child);
	}
}

QList<IProperty*> PropertyTable::properties() const
{
	return mProperties;
}

void PropertyTable::propertyWasDestroyed(IProperty* prop)
{
	Q_ASSERT(prop);
	remove(prop);
}

void PropertyTable::propertyWasChanged(QObject* obj)
{
	emit propertyChanged(static_cast<IProperty*>(obj));
}

void PropertyTable::propertyStructureWasChanged(QObject* obj)
{
	emit propertyStructureChanged(static_cast<IProperty*>(obj));
}

void PropertyTable::valueWasChanged(QObject* obj)
{
	emit valueChanged(static_cast<IProperty*>(obj));
}
