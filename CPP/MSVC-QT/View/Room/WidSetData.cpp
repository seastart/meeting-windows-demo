#include "WidSetData.h"
#include "ui_WidSetData.h"
#include "Tools/SetImageClass.h"
WidSetData::WidSetData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WidSetData)
{
    ui->setupUi(this);
    this->setWindowTitle("  ");

    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    this->setFixedSize(this->size());
}

WidSetData::~WidSetData()
{
    SetImageClass::StopPortraitImageDownLoad(_portrait);
    delete ui;
}

void WidSetData::SetUIData(QString userid,QString name,QString Portrait)
{
    _portrait = Portrait;
    SetImageClass::SetPortraitImage(ui->lblportrait,userid,Portrait,":/Images/avatar.png");
    QString nameString = name;
    QFontMetrics fontMetrics(ui->lblname->font());

    // 如果当前字体下，字符串长度大于label宽度
    if (fontMetrics.width(nameString) >318)
    {
        nameString = QFontMetrics(ui->lblname->font()).elidedText(nameString, Qt::ElideMiddle, 318);
    }
    ui->lblname->setText(nameString);
}

void WidSetData::on_pushButton_3_clicked()
{
    emit NameClicked();
    this->close();
}

