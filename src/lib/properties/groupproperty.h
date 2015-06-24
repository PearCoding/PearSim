#pragma once

#include "iproperty.h"

class PS_LIB GroupProperty : public IProperty
{
	Q_OBJECT
public:
	GroupProperty();
	virtual ~GroupProperty();

	QString valueText() const;
	void undo();
	void save();
	QWidget* editorWidget(QWidget* parent);
};
