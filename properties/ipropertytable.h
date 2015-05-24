#ifndef IPROPERTYTABLE_H
#define IPROPERTYTABLE_H

#include <QVariant>
#include <QString>
#include <QMap>

struct FeatureTable
{
    QString ParentKey;
};

class IPropertyTable
{
public:
    IPropertyTable();
    ~IPropertyTable();

    void add(const QString& key, const QVariant& value, const FeatureTable& features);

    void setValue(const QString& key, const QVariant& value);
    QVariant value(const QString& key) const;

    void setFeatures(const QString& key, const FeatureTable& features);
    FeatureTable features(const QString& key) const;

private:
    QMap<QString, QVariant> mProperties;
    QMap<QString, FeatureTable> mFeatures;
};

#endif // IPROPERTYTABLE_H
