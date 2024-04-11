#include "WidUserInfo.h"
#include "ui_WidUserInfo.h"
#include <QGraphicsDropShadowEffect>
#pragma execution_character_set("utf-8")
WidUserInfo::WidUserInfo(QWidget *parent) :
    TanChuWid(parent),
    ui(new Ui::WidUserInfo)
{
    ui->setupUi(this);
    this->SetType(0);
    this->SetMarginRadio(10,4);
    this->SetTriangle(16,10);
    this->SetMove(0.9);

    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(3, 3);              //阴影的偏移量
    shadow_effect->setColor(QColor(0x0, 0x0, 0x0, 0x1f)); //阴影的颜色
    shadow_effect->setBlurRadius(3);             // 阴影圆角的大小
    this->setGraphicsEffect(shadow_effect);


    QPushButton* btn = new QPushButton(this);
    //btn->SetKey(k);
    btn->setStyleSheet("border:0px;");
    btn->setText(tr("退出登录"));
    btn->setIcon(QIcon(":/Images/icon.png"));
    btn->setIconSize(QSize(20,20));
    btn->setFixedHeight(25);
    connect(btn,&QPushButton::clicked,[&](){
        emit BtnUnLoginClicked();
    });
    ui->verticalWidget->layout()->addWidget(btn);
}

WidUserInfo::~WidUserInfo()
{
    delete ui;
}

//void WidUserInfo::ClearBtnClick()
//{
//    int index = ui->verticalWidget->layout()->count();
//    for(int i = 0;i<index;i++)
//    {
//        QLayoutItem* layoutItem = ui->verticalWidget->layout()->itemAt(0);
//        QWidget* w = ((QWidget*)layoutItem->widget());
//        ui->verticalWidget->layout()->removeItem(layoutItem);
//        w->deleteLater();
//    }
//}
//#include <QDebug>
//void WidUserInfo::AddBtnClick(int k,QString img,QString txt)
//{
//QPushButton* btn = new QPushButton(this);
////btn->SetKey(k);
//btn->setStyleSheet("border:0px;");
//btn->setText(txt);
//btn->setIcon(QIcon(img));
//btn->setIconSize(QSize(20,20));
//btn->setFixedHeight(25);
//connect(btn,&QPushButton::clicked,[&,txt](){
//    emit BtnClicked(txt);
//});
//ui->verticalWidget->layout()->addWidget(btn);
//}
