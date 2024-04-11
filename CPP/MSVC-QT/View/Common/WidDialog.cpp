#include "WidDialog.h"
#include "ui_WidDialog.h"
#include <QDebug>
#include "../Tools/Utils.h"
#include <QRadioButton>
#include "ImageTipsWidgetWidget.h"
#pragma execution_character_set("utf-8")
WidDialog::WidDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidDialog)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);
    setAttribute(Qt::WA_DeleteOnClose, false);
    this->setWindowTitle("  ");
}

WidDialog::~WidDialog()
{
    delete ui;
}


void WidDialog::on_btnOk_clicked()
{
    qDebug()<<__func__<<ui->label->text();
    QString s = OutputData();
    if(_tp == DialogType::DialogType_Led){
        if(s.isEmpty()){
            ImageTipsWidgetWidget::DisplayDialog(this,tr("昵称不能为空"));
            return ;
        }
    }
    this->close();
    if(func_ok){
        func_ok(OutputData());
    }
    return ;
}


void WidDialog::on_btnCancel_clicked()
{
    qDebug()<<__func__<<ui->label->text();
    this->close();
    if(func_cancel){
        func_cancel(OutputData());
    }
    return ;
}
#include <QStyle>
void WidDialog::SetColor(DialogColor color){
    if(color == DialogColor::DialogColor_Red){
        ui->btnOk->setProperty("ShowType", "NormalRed");
        ui->btnCancel->setProperty("ShowType", "NormalCancelRed");

    }else{
        ui->btnOk->setProperty("ShowType", "Normal");
        ui->btnCancel->setProperty("ShowType", "NormalCancel");
    }
    ui->btnOk->style()->unpolish(ui->btnOk);
    ui->btnOk->style()->polish(ui->btnOk);
    ui->btnOk->update();

    ui->btnCancel->style()->unpolish(ui->btnCancel);
    ui->btnCancel->style()->polish(ui->btnCancel);
    ui->btnCancel->update();
}
void WidDialog::SetType(DialogType tp){
    _tp = tp;
}
void WidDialog::SetViewButton(QString ok,QString no,QString cancel){
    ui->btnOk->setText(ok);
    ui->btnCancel->setText(cancel);
}
void WidDialog::SetViewMsg(QString title,QString type_arg,QString type_arg2){
    ui->label->setText(title);
    ui->stackedWidget->setCurrentWidget(ui->page_label);
    ui->lblMsg->setText(type_arg);
}
void WidDialog::SetViewLed(QString title,QString type_arg,QString type_arg2){
    ui->label->setText(title);
    ui->stackedWidget->setCurrentWidget(ui->page_led);
    ui->lblLed->setPlaceholderText(type_arg2);
    ui->lblLed->setText(type_arg);
}
void WidDialog::SetViewRadio(QString title,QString type_arg,QString type_arg2){

    QVBoxLayout* flowLayout = ui->verticalLayout_2;
    int layoutListSize = flowLayout->count();
    for(auto btn : rbtns){
        btn->deleteLater();
    }
    for(int i = 0; i < layoutListSize; i++){
        ui->verticalLayout_2->removeItem(flowLayout->itemAt(0));
    }
    rbtns.clear();


    ui->label->setText(title);
    ui->stackedWidget->setCurrentWidget(ui->page_radios);
    QStringList sl = type_arg.split(",");
    bool isfind = false;
    for(auto s : sl){
        QHBoxLayout* layout = new QHBoxLayout();
        //layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        QRadioButton* rbtn = new QRadioButton(ui->page_radios);
        rbtn->setText(s);
        layout->addWidget(rbtn);
        layout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        ui->verticalLayout_2->addLayout(layout);
        if(s == type_arg2){
            rbtn->setChecked(true);
            isfind = true;
        }
        rbtns.append(rbtn);
    }
    if(!isfind && rbtns.size() > 0){
        rbtns.at(0)->setChecked(true);
    }
}
QString WidDialog::OutputData(){
    switch(_tp){
    case DialogType::DialogType_Led:
        return ui->lblLed->text();
    case DialogType::DialogType_Radio:
        {
            QString out;
            for(auto btn : rbtns){
                qDebug() << __func__ << btn->isChecked() << btn->text();
                if(btn->isChecked()){
                    out = btn->text();
                    break;
                }
            }
            return out;
        }
    }
    return "";
}


void WidDialog::SetFunc(std::function<bool(QString byte)> ok){
    func_ok = ok;
}
