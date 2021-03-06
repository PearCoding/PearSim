#pragma once

#include "iproperty.h"

#include <QColor>

class ColorButton;
class IntProperty;
class PS_LIB ColorProperty : public IProperty
{
	Q_OBJECT
public:
	ColorProperty();
	virtual ~ColorProperty();

	QString valueText() const;
	void undo();
	void save();
	QWidget* editorWidget(QWidget* parent);

	void setColor(const QColor& c);
	QColor color() const;

	void setDefaultColor(const QColor& c);
	QColor defaultColor() const;

private slots:
	void colorChanged(const QColor &);
	void dataChanged();

private: 
	ColorButton* mWidget;
	QColor mDefaultColor;
	QColor mColor;

	IntProperty* mRedProperty;
	IntProperty* mGreenProperty;
	IntProperty* mBlueProperty;
};
