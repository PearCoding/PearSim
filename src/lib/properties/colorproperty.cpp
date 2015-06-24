#include "colorproperty.h"

#include "utils/colorbutton.h"

ColorProperty::ColorProperty() :
IProperty(),
mWidget(nullptr),
mColor(Qt::white), mDefaultColor(Qt::white)
{
}

ColorProperty::~ColorProperty()
{
}

QString ColorProperty::valueText() const
{
	return mColor.name();
}

void ColorProperty::undo()
{
	setColor(mDefaultColor);
	setModified(false);
}

void ColorProperty::save()
{
	setDefaultColor(mColor);
	setModified(false);
}

QWidget* ColorProperty::editorWidget(QWidget* parent)
{
	if (!mWidget)
	{
		mWidget = new ColorButton(parent);

		mWidget->setEnabled(isEnabled());
		mWidget->setColor(mColor);

		connect(mWidget, SIGNAL(colorChanged(const QColor&)), this, SLOT(colorChanged(const QColor&)));
	}

	return mWidget;
}

void ColorProperty::colorChanged(const QColor& val)
{
	mColor = val;

	if (mColor != mDefaultColor && !isModified())
	{
		setModified(true);
	}
	else if (mColor == mDefaultColor && isModified())
	{
		setModified(false);
	}

	emit valueChanged();
}

void ColorProperty::setColor(const QColor& val)
{
	mColor = val;

	if (mColor != mDefaultColor && !isModified())
	{
		setModified(true);
	}
	else if (mColor == mDefaultColor && isModified())
	{
		setModified(false);
	}

	emit valueChanged();

	if (mWidget)
	{
		mWidget->setColor(mColor);
	}
}

QColor ColorProperty::color() const
{
	return mColor;
}

void ColorProperty::setDefaultColor(const QColor& val)
{
	mDefaultColor = val;

	if (mColor != mDefaultColor && !isModified())
	{
		setModified(true);
	}
	else if (mColor == mDefaultColor && isModified())
	{
		setModified(false);
	}
}

QColor ColorProperty::defaultColor() const
{
	return mDefaultColor;
}