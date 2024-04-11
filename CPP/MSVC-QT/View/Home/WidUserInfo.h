#ifndef WIDMEMBERMORE_H
#define WIDMEMBERMORE_H

#include <QWidget>

#include "View/Common/TanChuWid.h"
#include <QPushButton>

//class MemberMoreButton: public QPushButton
//{
//    Q_OBJECT
//public:
//    explicit MemberMoreButton(QWidget *parent = nullptr){}
//    ~MemberMoreButton(){}

//    void SetKey(int k){_key = k;}
//    int Key(){return _key;}
//private:
//    int _key;
//};

namespace Ui {
class WidUserInfo;
}

class WidUserInfo : public TanChuWid
{
    Q_OBJECT

public:
    explicit WidUserInfo(QWidget *parent = nullptr);
    ~WidUserInfo();

signals:
//    void BtnClicked(int,int);
    void BtnUnLoginClicked();
private:
    Ui::WidUserInfo*ui;
};

#endif // WIDMEMBERMORE_H
