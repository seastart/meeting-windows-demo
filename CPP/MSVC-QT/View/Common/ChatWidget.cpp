#include "ChatWidget.h"

#include <QDebug>
#include <QScrollBar>
#include "Global/GlobalDataClass.h"
#include "ImageTipsWidgetWidget.h"
#pragma execution_character_set("utf-8")
ChatWidget::ChatWidget(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);

    this->setWindowTitle("   ");
    qDebug() << __func__;
	ui.scrollArea->horizontalScrollBar()->hide();
	ui.content->installEventFilter(this);//窗口容器

    QPushButton * btnSend = new QPushButton(this);
    btnSend->setProperty("ShowType", "Normal");
    btnSend->setText(tr("发送"));
    btnSend->resize(80,40);
    btnSend->move(this->width() - 16 - btnSend->width()
                  ,this->height() - 12 - btnSend->height());
    connect(btnSend,SIGNAL(clicked()),this,SLOT(OnbtnSendClicked()));

    ui.widTarget->hide();
    this->setFixedSize(this->size());
}

ChatWidget::~ChatWidget()
{

    qDebug() << __func__;
}
QWidget* ChatWidget::GetViewWidget()
{
    return ui.content;
}
void ChatWidget::SetViewLayout(QLayout* l)
{
    ui.content->setLayout(l);
}

bool ChatWidget::eventFilter(QObject* obj, QEvent* event)
{//自动滚到到最底部
	static int nHeight = 0;
	if (dynamic_cast<QWidget*>(obj) == ui.content)
	{
		if (event->type() == QEvent::Resize)
		{
			int nDif = ui.content->height();
			if (nDif > 0)
			{//容器高度大于滚动区域，此时就需要滚动条
				QScrollBar* pVScrollBar = ui.scrollArea->verticalScrollBar();
				pVScrollBar->setMaximum(nDif);//设置最大值
				pVScrollBar->setValue(nDif);//将当前值设置成最大值
				return false;
			}
		}
	}
	return QWidget::eventFilter(obj, event);
}

void ChatWidget::on_btnClose_clicked()
{
    this->hide();
}

void ChatWidget::OnbtnSendClicked()
{
    QString msg = ui.ledChat->toPlainText();
    qDebug() << __func__ << msg;
    QString data = msg;
    if (data.isEmpty())
    {
        return;
    }
    QRegExp rx("\\S");
    int rx_index = rx.indexIn(msg);
    if(rx_index >=0){
        emit SendMsg(ui.ledTar->text(),data);
    }else{
        ImageTipsWidgetWidget::DisplayDialog(this,tr("不能发送空白信息"));
    }
    ui.ledChat->setPlainText("");
}
