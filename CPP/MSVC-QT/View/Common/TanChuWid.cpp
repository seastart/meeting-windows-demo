#include "TanChuWid.h"
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
#ifdef Q_OS_WIN
#include "windows.h"
#endif
TanChuWid::TanChuWid(QWidget *parent)
    : QWidget(parent)
{
    radius = 16;
    color = "#ffffff";
    margin = 10;
    size_height = 10;    //突出的大小
    size_width  = 15;    //水平/垂直大小
    target = 0;         //0123 上下左右
    _move = 0.5;        //比例
    this->setWindowFlag(Qt::Popup);
    this->setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);//消失的时候就会进行资源释放。
}

TanChuWid::~TanChuWid()
{
}

void TanChuWid::SetMarginRadio(int margin,int radius)
{
    this->margin = margin;
    this->radius = radius;
}

void TanChuWid::SetBackColor(QString color)
{
    this->color = color;
}
void TanChuWid::SetMove(double p)
{
    this->_move = p;
}

void TanChuWid::SetType(int type)
{
    target = type;
}

void TanChuWid::SetTriangle(int wid,int h)
{
    this->size_height = h;
    this->size_width = wid;
}

 #ifdef Q_OS_WIN
 bool TanChuWid::event(QEvent* event)
 {
     static bool class_amended = false;
     if (event->type() == QEvent::WinIdChange)
     {
         HWND hwnd = reinterpret_cast<HWND>(winId());
         if (class_amended == false)
         {
             class_amended = true;
             DWORD class_style;
             class_style = ::GetClassLong(hwnd, GCL_STYLE);
             class_style &= ~CS_DROPSHADOW;
             ::SetClassLong(hwnd, GCL_STYLE, class_style); // windows系统函数
         }
     }
     return QWidget::event(event);
 }
 #endif

void TanChuWid::paintEvent(QPaintEvent *event)
{
//    int radius = 16;
//    QString color = "#ff0000";
//    int margin = 10;
//    int size_height =10;    //突出的大小
//    int size_width  =15;    //水平/垂直大小
//    int target = 3;         //0123 上左下右
//    double move = 0;        //比例

    QRect rect(QPoint(margin,margin),QSize(this->size().width() - margin*2,this->size().height() - margin*2));
    QList<QPoint> lp;
    switch(target)
    {
        case 0:
        {
            int m = _move * (this->width() - 2*margin - 2*radius- size_width) ;
            lp.append(QPoint(margin + m + radius,margin+0));
            lp.append(QPoint(margin + m + size_width + radius,margin+0));
            lp.append(QPoint(margin + m + (size_width)/2 + radius ,margin-size_height + 0));
        break;
        }
        case 1:
        {
        int n = _move * (this->height() - 2*margin - 2*radius- size_width) ;
        lp.append(QPoint(margin + 0 ,margin+n+ radius));
        lp.append(QPoint(margin + 0 ,margin+n +size_width + radius));
        lp.append(QPoint(margin + 0 - size_height,margin + (size_width)/2 + n + radius));
        break;
        }
        case 2:
        {
        int m = _move * (this->width() - 2*margin - 2*radius- size_width) ;
        int n = this->height() - 2*margin;
        lp.append(QPoint(margin + m + radius,margin+n));
        lp.append(QPoint(margin + m + size_width + radius,margin+n));
        lp.append(QPoint(margin + m + (size_width)/2 + radius ,margin + size_height+n));
        break;
        }
        case 3:
        {
        int m = this->width() - 2*margin;
        int n = _move * (this->height() - 2*margin - 2*radius- size_width) ;
        lp.append(QPoint(margin + m ,margin+n+ radius));
        lp.append(QPoint(margin + m ,margin+n +size_width + radius));
        lp.append(QPoint(margin + m + size_height,margin + (size_width)/2 + n + radius));
        break;
        }
    }
    QPainter painter(this);
    painter.setPen(QColor(color));    //画笔
    painter.setBrush(QColor(color));   // 画刷

    QPolygon triangle;
    triangle.setPoints(3,lp[0].x(),lp[0].y(),lp[1].x(),lp[1].y(),lp[2].x(),lp[2].y());//三点坐标
    painter.drawPolygon(triangle);    //画三角形
    painter.drawRoundedRect(rect, radius, radius);
}
