#pragma once

#include "iproperty.h"

#include <QVector>
#include <QVariant>

class QComboBox;
class PS_LIB SelectionProperty : public IProperty
{
	Q_OBJECT
public:
	SelectionProperty();
	virtual ~SelectionProperty();

	QString valueText() const;
	void undo();
	void save();
	QWidget* editorWidget(QWidget* parent);

	void setIndex(int i);
	int index() const;

	void setDefaultIndex(int i);
	int defaultIndex() const;

	void addItem(const QString& text, const QVariant& userData = QVariant());
	void removeItem(int index);

private slots:
	void comboChanged(int i);

private: 
	QComboBox* mWidget;
	int mDefaultIndex;
	int mIndex;

	QVector<QPair<QString, QVariant> > mData;
};
