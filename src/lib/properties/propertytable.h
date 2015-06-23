#pragma once

#include <QList>
#include <QString>
#include <QSignalMapper>

#include "iproperty.h"

class PS_LIB PropertyTable : public QObject
{
	Q_OBJECT
public:
	PropertyTable();
	~PropertyTable();

	void add(IProperty* property);
	void remove(IProperty* property);
	QList<IProperty*> properties() const;

signals:
	void propertyChanged(IProperty* prop);
	void propertyStructureChanged(IProperty* prop);
	void valueChanged(IProperty* prop);

private slots:
	void propertyWasDestroyed(IProperty* prop);
	void propertyWasChanged(QObject* obj);
	void propertyStructureWasChanged(QObject* obj);
	void valueWasChanged(QObject* obj);

private:
	QList<IProperty*> mProperties;
	QSignalMapper mPropertyChangedMapper;
	QSignalMapper mPropertyStructureChangedMapper;
	QSignalMapper mValueChangedMapper;
};
