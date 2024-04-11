#include "WidgetClick.h"

WidgetClick::WidgetClick(QWidget* parent)
	: QWidget(parent)
{
}

WidgetClick::~WidgetClick()
{
}

void WidgetClick::mousePressEvent(QMouseEvent* e)
{
	emit Clicked();
	QWidget::mousePressEvent(e);
}
