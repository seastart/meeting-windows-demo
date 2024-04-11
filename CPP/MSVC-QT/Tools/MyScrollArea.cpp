#include "MyScrollArea.h"

MyScrollArea::MyScrollArea(QWidget *parent)
    : QScrollArea{parent}
{

}

void MyScrollArea::scrollContentsBy(int dx, int dy)
{
    QScrollArea::scrollContentsBy(dx,dy);
    emit WheelChange();
}
