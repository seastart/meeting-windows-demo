#ifndef MYSCROLLAREA_H
#define MYSCROLLAREA_H

#include <QWidget>
#include <QScrollArea>

class MyScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    explicit MyScrollArea(QWidget *parent = nullptr);

protected:
    void scrollContentsBy(int dx, int dy);
signals:
    void WheelChange();
};

#endif // MYSCROLLAREA_H
