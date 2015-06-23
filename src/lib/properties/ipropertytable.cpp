#include "ipropertytable.h"

IPropertyTable::IPropertyTable()
{

}

IPropertyTable::~IPropertyTable()
{

}

void IPropertyTable::add(const QString& key, const QVariant& value, const PropertyDefinition& def)
{
    mProperties[key] = value;
    mDefinitions[key] = def;
}

void IPropertyTable::addAction(const ActionDefinition& def)
{
    mActions.append(def);
}

void IPropertyTable::setValue(const QString& key, const QVariant& value)
{
    if(!mProperties.contains(key))
    {
        PropertyDefinition def;
        def.ParentKey = "";

        mDefinitions[key] = def;
    }

    mProperties[key] = value;
}

QVariant IPropertyTable::value(const QString& key) const
{
    return mProperties[key];
}

void IPropertyTable::setPropertyDefinition(const QString& key, const PropertyDefinition& def)
{
    Q_ASSERT(mProperties.contains(key));

    mDefinitions[key] = def;
}

PropertyDefinition IPropertyTable::propertyDefinition(const QString& key) const
{
    return mDefinitions[key];
}
