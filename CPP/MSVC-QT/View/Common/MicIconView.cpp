#include "MicIconView.h"
MicIconView::MicIconView(QWidget* parent)
    : QWidget(parent)
{
}

MicIconView::~MicIconView() 
{

}
int old_f_db = -99;
void MicIconView::AddDb(int db) 
{
    int f_db = 60 + db;//(0 - 60) 数值越大声音越大
    if (f_db >= 0 && old_f_db != f_db) {
        old_f_db = f_db;
        h = (f_db * (this->height() * 3 / 5))/60;
        update();
    }
}
#include <QPainter>
#include <QPainterPath>
void MicIconView::paintEvent(QPaintEvent* event) 
{

    QPainter painter1(this);


    int mic_h = this->height() * 31 / 45;
    int mic_w = this->width() *2/5;
    QColor cback = _backColor;// QColor(0xff, 0xff, 0xff);//QColor(0xdd, 0xdd, 0xdd, 255);
//    painter1.drawRect(0,0,this->width(),this->height());


    painter1.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor(cback);
    pen.setWidth(mic_w / 4);
    painter1.setPen(pen);
    QRect rt((this->width()) / 2 - (mic_w), mic_h / 4, mic_w * 2, mic_h);
    painter1.drawArc(rt, 10 * 16, -200 * 16);
    QLine l((this->width()) / 2 - (mic_w), this->height() - pen.width(), (this->width()) / 2 - (mic_w)+mic_w * 2, this->height() - pen.width());
    painter1.drawLine(l);
    QLine ll((this->width()) / 2, mic_h + mic_h / 4, (this->width()) / 2, this->height() - pen.width());
    painter1.drawLine(ll);
    //    painter1.drawRect(rt);
    painter1.setPen(Qt::PenStyle::NoPen);
    if (openclose) {
        QRect rect;
        rect.setTop(0);
        rect.setLeft((this->width() - mic_w) / 2);
        rect.setWidth(mic_w);
        rect.setHeight(mic_h);
        QPainterPath path;
        path.addRoundedRect(rect, mic_w / 2, mic_w / 2);
        painter1.setClipPath(path);

        painter1.setBrush(cback);
        painter1.drawRect((this->width() - mic_w) / 2, 0, mic_w, mic_h - 1);
        painter1.setBrush(QColor(0, 200, 50, 255));
        painter1.drawRect((this->width() - mic_w) / 2, mic_h - h, mic_w, h);
    }
    else {
        painter1.setBrush(cback);
        QRect rect;
        rect.setTop(0);
        rect.setLeft((this->width() - mic_w) / 2);
        rect.setWidth(mic_w);
        rect.setHeight(mic_h);
        painter1.drawRoundedRect(rect, mic_w / 2, mic_w / 2);

        pen.setColor(QColor(255, 68, 67));
        pen.setWidth(mic_w / 15);
        painter1.setPen(pen);

        painter1.drawLine(this->width() * 4 / 5, this->height() / 7, this->width() / 5, this->height() * 6 / 7);
    }
    return;
}
