#include "ImageTipsWidgetWidget.h"
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QDebug>
#include "Tools/Utils.h"

ImageTipsWidgetWidget::ImageTipsWidgetWidget(QWidget* parent)
	: QWidget(parent)
{
    ui.setupUi(this);
}

ImageTipsWidgetWidget::~ImageTipsWidgetWidget()
{
}

void ImageTipsWidgetWidget::SetImageText(QString img, QString txt)
{
	if (!img.isEmpty())
	{
		ui.label_2->setPixmap(QPixmap(img));
	}
	else
	{
		ui.label_2->hide();
		ui.label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	}
  //  ui.label->setText(txt);
    WrapData(txt,ui.label);
    qDebug()<<__func__<<ui.label->text();
}

void ImageTipsWidgetWidget::WrapData(const QString& text,QLabel* lbl)
{
    QFontMetrics font(lbl->font());
    int allLen = font.width(text);
    int uiLen = this->maximumWidth() - 120;
    QString ftxt = "";
    QString t = text;
    if(uiLen < allLen)
    {
        while(!t.isEmpty())
        {
            int index = 0;
            if(!ftxt.isEmpty())
            {
                ftxt += "\n";
            }
            ftxt +=SpliteText(font,t,uiLen,index);
            t.remove(0,index);
        }
    }else
    {
        ftxt = text;
    }

    lbl->setText(ftxt);
}

QString ImageTipsWidgetWidget::SpliteText(const QFontMetrics& font, const QString& text, int nLabelSize,int& index)
{
    int i = 1;

    while(font.width(text.mid(0,i)) < nLabelSize)
    {
        if(i == text.size())
        {
            break;
        }
        i++;

    }
    index = i;
    return text.mid(0,index);
}

void ImageTipsWidgetWidget::StartHideAnimation(int dur)
{
	QGraphicsOpacityEffect* pButtonOpacity = new QGraphicsOpacityEffect(this);
	pButtonOpacity->setOpacity(0);
	this->setGraphicsEffect(pButtonOpacity);

	QPropertyAnimation* pOpacityAnimation1 = new QPropertyAnimation(pButtonOpacity, "opacity");
	pOpacityAnimation1->setDuration(dur);
	pOpacityAnimation1->setStartValue(0);
	pOpacityAnimation1->setEndValue(1);


	QPropertyAnimation* pOpacityAnimation12 = new QPropertyAnimation(pButtonOpacity, "opacity");
	pOpacityAnimation12->setDuration(dur);
	pOpacityAnimation12->setStartValue(1);
	pOpacityAnimation12->setEndValue(0);


	QSequentialAnimationGroup* sequGroup = new QSequentialAnimationGroup(this);
	sequGroup->addAnimation(pOpacityAnimation1);
	sequGroup->addPause(1000);
	sequGroup->addAnimation(pOpacityAnimation12);
	connect(sequGroup, SIGNAL(finished()), this, SLOT(OnFinished()));
	sequGroup->start();
}

void ImageTipsWidgetWidget::OnFinished()
{
	this->deleteLater();
}

void ImageTipsWidgetWidget::Display(QWidget* widget, const QString text, const int imgindex)
{
	qDebug() << __func__ << ",msg:" << text << ",index:" << imgindex;
	QString img;
	switch (imgindex) {
	case 1:
		img = (":/image/Image/success.png");
		break;
	case 0:
		img = (":/image/Image/error.png");
		break;
	}
	ImageTipsWidgetWidget::Display(widget, text, img);
}

void ImageTipsWidgetWidget::Display(QWidget* widget, const QString text, const QString img)
{
	widget = Utils::GetTopWidget(widget);

	ImageTipsWidgetWidget* tips = new ImageTipsWidgetWidget();
	tips->SetImageText(img, text);
	tips->setParent(widget);
	tips->show();
	tips->raise();
	tips->adjustSize();

	QSize size = widget->size();

	float width = tips->size().width();
	float height = tips->size().height();

	float x_pos = (size.width() - width) / 2.f;
	float y_pos = (size.height() - height) / 2.f;

	tips->move(x_pos, y_pos);
	tips->StartHideAnimation(1000);
}


void ImageTipsWidgetWidget::DisplayDialog(QWidget* widget, const QString text, const int imgindex)
{
	qDebug() << __func__ << text;
	QString img;
	switch (imgindex) {
	case 1:
		img = (":/image/Image/success.png");
		break;
	case 0:
		img = (":/image/Image/error.png");
		break;
	case -1:
		img = (":/image/Image/i.png");
		break;
	}

	ImageTipsWidgetWidget* tips = new ImageTipsWidgetWidget();
	tips->setAttribute(Qt::WA_TranslucentBackground);
    tips->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#ifdef Q_OS_WIN
    tips->setWindowFlag(Qt::Tool);
    tips->raise();
#endif
	tips->SetImageText(img, text);

    tips->show();
	tips->adjustSize();
	tips->StartHideAnimation(1000);
	if (widget)
	{
		QPoint p = widget->mapToGlobal(QPoint(0, 0));
		tips->move(p.x() + (widget->width() - tips->width()) / 2, p.y() + (widget->height() - tips->height()) / 2);
		//tips->setParent(widget);
	}
	//if (curDisplayMoveIndex > 1)
	//{
	//    if (curDisplayMoveIndex > 10)
	//    {
	//        curDisplayMoveIndex %= 10;
	//    }
	//    int top = tips->y() - 50* (curDisplayMoveIndex - 1);
	//    if (top < 0)
	//    {
	//        top = 0;
	//    }
	//    tips->move(tips->x(), top);
	//}
}
