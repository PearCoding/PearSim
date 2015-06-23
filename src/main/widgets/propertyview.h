#pragma once

#include <QWidget>

class PropertyView : public QWidget
{
	Q_OBJECT
public:
	explicit PropertyView(QWidget* parent = nullptr, Qt::WindowFlags f = 0);
	virtual ~PropertyView();
};
