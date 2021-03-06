#include "intproperty.h"

#include <QSpinBox>

IntProperty::IntProperty() :
IProperty(),
mSpinBox(nullptr),
mValue(0), mOldValue(0),
mMaxValue(100000), mMinValue(-100000), mStepSize(1)
{
}

IntProperty::~IntProperty()
{
}

QString IntProperty::valueText() const
{
	return QString::number(mValue);
}

void IntProperty::undo()
{
	setValue(mOldValue);
	setModified(false);
}

void IntProperty::save()
{
	setDefaultValue(mValue);
	setModified(false);
}

QWidget* IntProperty::editorWidget(QWidget* parent)
{
	if (!mSpinBox)
	{
		mSpinBox = new QSpinBox(parent);

		mSpinBox->setEnabled(isEnabled());
		mSpinBox->setReadOnly(isReadOnly());
		mSpinBox->setValue(mValue);
		mSpinBox->setMaximum(mMaxValue);
		mSpinBox->setMinimum(mMinValue);
		mSpinBox->setSingleStep(mStepSize);

		connect(mSpinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxChanged(int)));
	}

	return mSpinBox;
}

void IntProperty::spinBoxChanged(int val)
{
	mValue = val;

	if (mValue != mOldValue && !isModified())
	{
		setModified(true);
	}
	else if (mValue == mOldValue && isModified())
	{
		setModified(false);
	}

	emit valueChanged();
}

void IntProperty::setValue(int val)
{
	mValue = qMax(mMinValue, qMin(mMaxValue, val));

	if (mValue != mOldValue && !isModified())
	{
		setModified(true);
	}
	else if (mValue == mOldValue && isModified())
	{
		setModified(false);
	}

	emit valueChanged();

	if (mSpinBox)
	{
		mSpinBox->setValue(mValue);
	}
}

int IntProperty::value() const
{
	return mValue;
}

void IntProperty::setDefaultValue(int val)
{
	mOldValue = qMax(mMinValue, qMin(mMaxValue, val));

	if (mValue != mOldValue && !isModified())
	{
		setModified(true);
	}
	else if (mValue == mOldValue && isModified())
	{
		setModified(false);
	}
}

int IntProperty::defaultValue() const
{
	return mOldValue;
}

int IntProperty::maxValue() const
{
	return mMaxValue;
}

int IntProperty::minValue() const
{
	return mMinValue;
}

int IntProperty::stepSize() const
{
	return mStepSize;
}

void IntProperty::setMaxValue(int i)
{
	mMaxValue = i;

	if (mSpinBox)
	{
		mSpinBox->setMaximum(i);
	}
}

void IntProperty::setMinValue(int i)
{
	mMinValue = i;

	if (mSpinBox)
	{
		mSpinBox->setMinimum(i);
	}
}

void IntProperty::setStepSize(int i)
{
	mStepSize = i;

	if (mSpinBox)
	{
		mSpinBox->setSingleStep(i);
	}
}