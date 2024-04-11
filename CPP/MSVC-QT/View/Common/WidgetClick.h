#pragma once

#include <QWidget>
#include <QMouseEvent>
class WidgetClick : public QWidget
{
	Q_OBJECT

protected:
	void mousePressEvent(QMouseEvent* event);
signals:
	void Clicked();
public:
	WidgetClick(QWidget* parent);
	~WidgetClick();
};
