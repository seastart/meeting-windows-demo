#include "RoundLoadingWidget.h"

RoundLoadingWidget::RoundLoadingWidget(QWidget* parent) :
	QWidget(parent)
	, tim(nullptr)
{
	//给变量设置初始值-默认值
	startAngle = -90;
	endAngle = 270;
	ringWidth = 4;
	maxRadius = 120;
	iconWidth = 3;
	currAngle = 1;
	circularBorderColor = qRgb(255, 255, 255);
	insideMaskColor = qRgb(255, 255, 255);
	startGradientColor = qRgb(255, 255, 255);
	endGradientColor = qRgb(0x3e, 0x80, 0xff);

	tim = new QTimer(this);
	tim->start(30);
	connect(tim, SIGNAL(timeout()), this, SLOT(update()));
}

void RoundLoadingWidget::SetStartAngle(int value)
{
	startAngle = value;
}

void RoundLoadingWidget::SetEndAngle(int value)
{
	endAngle = value;
}

void RoundLoadingWidget::SetMaxRadius(int value)
{
	maxRadius = value;
}

void RoundLoadingWidget::SetRingWidth(int value)
{
	ringWidth = value;
}

void RoundLoadingWidget::SetIcomWidth(int value)
{
	iconWidth = value;
}

void RoundLoadingWidget::SetCircularBorderColor(QRgb value)
{
	circularBorderColor = value;
}

void RoundLoadingWidget::SetInsideMaskColor(QRgb value)
{
	insideMaskColor = value;
}

void RoundLoadingWidget::SetRadialGradient(QRgb startColor, QRgb endColor)
{
	startGradientColor = startColor;
	endGradientColor = endColor;
}

void RoundLoadingWidget::SetCurrPrograss(int value)
{
	currAngle = value;
	update();
}


void RoundLoadingWidget::resizeEvent(QResizeEvent* event)
{
	maxRadius = event->size().width() / 2;
}

void RoundLoadingWidget::paintEvent(QPaintEvent*)
{
	static int i = 0;//定义旋转度数
	i += 4;
	//初始化变量
	QPainter painter(this);
	// 右移1位 相当于width()/2
	painter.translate(width() >> 1, height() >> 1);
	//启动反锯齿
	painter.setRenderHint(QPainter::Antialiasing);



	painter.translate(0, 0);//设置原点为窗口中心
	painter.rotate(i);//坐标旋转
	if (i >= 360)
		i = 0;//旋转度数检测

		//绘制外圆
	DrawOutSideCircle(painter);
	//    //绘制两个小圆外圆
	//    DrawTwoSmallCircle(painter,0);
		//绘制圆环
	//    DrawCircularRing(painter,0);
		//绘制两个小圆内圆
	//    DrawTwoSmallCircle(painter,1);
		//绘制渐变色圆环
	DrawGradientCircle(painter, maxRadius - 1);
	//绘制内圆
	DrawInSideCircle(painter);
	//绘制进度小圆
	DrawBarIcon(painter);
}

void RoundLoadingWidget::GradientArc(QPainter& painter, int radius, QRgb color)
{
	// 渐变色
	QRadialGradient gradient(0, 0, radius);
	gradient.setColorAt(0, color);
	gradient.setColorAt(1.0, color);
	painter.setBrush(gradient);

	QRectF rect(-radius, -radius, radius << 1, radius << 1);
	QPainterPath path;
	path.arcTo(rect, startAngle, endAngle - startAngle);

	painter.setPen(Qt::NoPen);
	painter.drawPath(path);
}

void RoundLoadingWidget::GradientFullArc(QPainter& painter, QPointF& point, int radius, QRgb color)
{
	// 渐变色
	QRadialGradient gradient(0, 0, radius);
	gradient.setColorAt(0, color);
	gradient.setColorAt(1.0, color);
	painter.setBrush(gradient);
	painter.drawEllipse(point, radius, radius);
}

void RoundLoadingWidget::DrawOutSideCircle(QPainter& painter)
{
	GradientArc(painter, maxRadius, circularBorderColor);
}

void RoundLoadingWidget::DrawCircularRing(QPainter& painter, int)
{
	GradientArc(painter, maxRadius - 1, startGradientColor);
}

void RoundLoadingWidget::DrawTwoSmallCircle(QPainter& painter, int type)
{
	//计算小圆坐标
	QPoint rightCircle(0, 0);
	QPoint leftCircle(0, 0);
	rightCircle.setY(-(qSin(startAngle * M_PI / 180) * (maxRadius - ringWidth / 2)));
	rightCircle.setX(qCos(startAngle * M_PI / 180) * (maxRadius - ringWidth / 2) + 1);
	leftCircle.setX(-rightCircle.rx());
	leftCircle.setY(rightCircle.ry());
	if (type == 0) {
		painter.drawEllipse(rightCircle, ringWidth / 2, ringWidth / 2);
		painter.drawEllipse(leftCircle, ringWidth / 2, ringWidth / 2);
	}
	else {
		painter.drawEllipse(rightCircle, ringWidth / 2 - 1, ringWidth / 2 - 1);
		painter.drawEllipse(leftCircle, ringWidth / 2 - 1, ringWidth / 2 - 1);
	}
}

void RoundLoadingWidget::DrawInSideCircle(QPainter& painter)
{
	QPointF point(0, 0);
	GradientArc(painter, maxRadius - ringWidth + 1, circularBorderColor);
	GradientFullArc(painter, point, maxRadius - ringWidth, insideMaskColor);
	//GradientFullArc(painter,maxRadius*3/4,qRgb(48,48,48));
}

void RoundLoadingWidget::DrawGradientCircle(QPainter& painter, int radius)
{
	QConicalGradient conicalGradient(QPointF(0, 0), endAngle + 1);
	conicalGradient.setColorAt((360 - qAbs(endAngle - startAngle) + currAngle) / 360.0, endGradientColor);
	conicalGradient.setColorAt(1, startGradientColor);
	painter.setBrush(conicalGradient);
	QPainterPath path;
	QRectF rect(-radius, -radius, radius << 1, radius << 1);
	path.arcTo(rect, startAngle + currAngle, endAngle - startAngle - currAngle);
	painter.setPen(Qt::NoPen);
	painter.drawPath(path);
}

void RoundLoadingWidget::DrawBarIcon(QPainter& painter)
{
	QPointF point(0, 0);
	point.setX(point.x() + qCos((startAngle + currAngle) * M_PI / 180) * (maxRadius - ringWidth / 2));
	point.setY(point.y() - qSin((startAngle + currAngle) * M_PI / 180) * (maxRadius - ringWidth / 2));
	//    GradientFullArc(painter,point,iconWidth,endGradientColor);
	GradientFullArc(painter, point, iconWidth * 0.6, endGradientColor);
}
#include <QDebug>
void RoundLoadingWidget::SetTimerEnable(bool v)
{
	if (tim)
	{
		if (v == tim->isActive())
		{
			return;
		}
        qDebug()<<__func__<<v;
		if (v)
        {
            tim->start();
		}
		else
        {
            tim->stop();
		}
	}
}
