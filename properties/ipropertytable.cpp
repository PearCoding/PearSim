#include "ipropertytable.h"

IPropertyTable::IPropertyTable()
{

}

IPropertyTable::~IPropertyTable()
{

}

void IPropertyTable::add(const QString& key, const QVariant& value, const FeatureTable& features)
{
    mProperties[key] = value;
    mFeatures[key] = features;
}

void IPropertyTable::setValue(const QString& key, const QVariant& value)
{
    if(!mProperties.contains(key))
    {
        FeatureTable features;
        features.ParentKey = "";

        mFeatures[key] = features;
    }

    mProperties[key] = value;
}

QVariant IPropertyTable::value(const QString& key) const
{
    return mProperties[key];
}

void IPropertyTable::setFeatures(const QString& key, const FeatureTable& features)
{
    Q_ASSERT(mProperties.contains(key));

    mFeatures[key] = features;
}

FeatureTable IPropertyTable::features(const QString& key) const
{
    return mFeatures[key];
}
