#include "WidBtn.h"
#include "ui_WidBtn.h"

WidBtn::WidBtn(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::WidBtn)
{
    ui->setupUi(this);
}

WidBtn::~WidBtn()
{
    delete ui;
}

void WidBtn::SetIconSize(int size)
{
    ui->lblImg->setFixedSize(size, size);
}

void WidBtn::setText(const QString &text){
    ui->lbltitle->setText(text);
}
void WidBtn::setImg(QString i){
    ui->lblImg->setPixmap(QPixmap(i));

}
void WidBtn::setImgText(QString t,QString i){

    ui->lbltitle->setText(t);
    ui->lblImg->setPixmap(QPixmap(i));
}
