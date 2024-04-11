#ifndef TANCHUWID_H
#define TANCHUWID_H

#include <QWidget>

class TanChuWid : public QWidget
{
    Q_OBJECT

public:
    TanChuWid(QWidget *parent = nullptr);
    ~TanChuWid();
    void SetMarginRadio(int margin,int radius);
    void SetBackColor(QString color);
    void SetMove(double p);
    void SetType(int type);
    void SetTriangle(int wid,int h);
protected:
    void paintEvent(QPaintEvent *event);
 #ifdef Q_OS_WIN
     bool event(QEvent* event) override;
 #endif
private:
    int radius = 16;
    QString color = "#ff0000";
    int margin = 10;
    int size_height =10;    //突出的大小
    int size_width  =15;    //水平/垂直大小
    int target = 3;         //0123 上下左右
    double _move = 0;        //比例
};
#endif // TANCHUWID_H
