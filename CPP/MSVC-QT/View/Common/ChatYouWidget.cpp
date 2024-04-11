#include "ChatYouWidget.h"
#include "ui_ChatYouWidget.h"
#include "Tools/SetImageClass.h"
#include <QClipboard>
#include <QDebug>
#pragma execution_character_set("utf-8")
ChatYouWidget::ChatYouWidget(QWidget *parent) :
    IChatBase(parent),
    ui(new Ui::ChatYouWidget)
{
    ui->setupUi(this);
    qDebug() << __func__;
    ui->lblMsg->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->lblMsg->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->lblMsg->setTextFormat(Qt::TextFormat::PlainText);
}

ChatYouWidget::~ChatYouWidget()
{
    qDebug() << __func__;
    SetImageClass::StopPortraitImageDownLoad(_portrait);
        delete ui;
}

void ChatYouWidget::SetNickName(QString Name)
{
    qDebug() << __func__;
    QFontMetrics fontMetrics(ui->lblName->font());
    QString nameString = Name;
    if (fontMetrics.width(nameString) > 200)
    {
        nameString = QFontMetrics(ui->lblName->font()).elidedText(nameString, Qt::ElideRight, 200);
    }
    //设置名称
    ui->lblName->setText(nameString);
    ui->lblName->adjustSize();
}

void ChatYouWidget::SetData(QString id, QString Name, QString portrait, QString message, bool isShowHead)
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
        ui->lblName->setText("");
	}
    WrapData(message,ui->lblMsg);
}

void ChatYouWidget::WrapData(const QString& text,QLabel* lbl)
{
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
    lbl->setText(ftxt);
    qDebug()<<__func__<<ftxt<<1<<lbl->size();
}

QString ChatYouWidget::SpliteText(const QFontMetrics& font, const QString& text, int nLabelSize,int& index,bool& n)
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
