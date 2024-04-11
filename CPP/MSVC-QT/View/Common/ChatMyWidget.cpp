#include "ChatMyWidget.h"
#include "ui_ChatMyWidget.h"

#include <QClipboard>
#include <QDebug>
#include "Tools/SetImageClass.h"
#pragma execution_character_set("utf-8")
ChatMyWidget::ChatMyWidget(QWidget* parent)
    :IChatBase(parent),
    ui(new Ui::ChatMyWidget)
{
    ui->setupUi(this);

    qDebug() << __func__;
    ui->lblMsg->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->lblMsg->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lblMsg->setTextFormat(Qt::TextFormat::PlainText);
}

ChatMyWidget::~ChatMyWidget()
{
    qDebug() << __func__;
    SetImageClass::StopPortraitImageDownLoad(_portrait);
    delete ui;
}
void ChatMyWidget::SetNickName(QString Name)
{

    qDebug() << __func__;
}

void ChatMyWidget::SetData(QString id, QString Name, QString portrait, QString message, bool isShowHead)
{
    _portrait = portrait;
    qDebug() << __func__;
    if (isShowHead)
    {
        //设置头像
        SetImageClass::SetPortraitImage(ui->lblportrait, id, portrait, ":/Images/avatar.png");
        SetNickName(Name);
    }
    else
    {
        ui->lblportrait->clear();
    }
    WrapData(message,ui->lblMsg);
}

void ChatMyWidget::WrapData(const QString& text,QLabel* lbl)
{
    qDebug() << __func__;
    QFontMetrics font(lbl->font());
    int allLen = font.width(text);
    int uiLen = lbl->maximumWidth();
    QString ftxt = "";
    QString t = text;
    if(uiLen < allLen)
    {
        bool isn = false;
        while(!t.isEmpty())
        {
            int index = 0;
            if(!ftxt.isEmpty() && (t != text && t[0] != '\n'))
            {
                if(!isn){
                    ftxt += "\n";
                }
            }

            ftxt +=SpliteText(font,t,uiLen,index,isn);
            t.remove(0,index);
        }
    }else
    {
        ftxt = text;
    }
    qDebug()<<__func__<<ftxt;
    lbl->setText(ftxt);
}

QString ChatMyWidget::SpliteText(const QFontMetrics& font, const QString& text, int nLabelSize,int& index,bool& n)
{
    int i = 1;
    n = false;
    while(font.width(text.mid(0,i)) < nLabelSize)
    {
        if(i == text.size())
        {
            break;
        }
        if(text.at(i) == '\n'){
            i++;
            n = true;
            break;
        }
        i++;
    }
    index = i;
    return text.mid(0,index);
}
