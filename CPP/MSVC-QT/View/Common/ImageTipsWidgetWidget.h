#pragma once

#include <QWidget>
#include "ui_ImageTipsWidgetWidget.h"

class ImageTipsWidgetWidget : public QWidget
{
	Q_OBJECT

public:
	ImageTipsWidgetWidget(QWidget* parent = Q_NULLPTR);
	~ImageTipsWidgetWidget();

	//1 成功 0 失败 -1 消息（默认）
	static void Display(QWidget* widget, const QString text, const int img = -2);
	static void Display(QWidget* widget, const QString text, const QString img);
	static void DisplayDialog(QWidget* widget, const QString text, const int img = -2);

	void SetImageText(QString img, QString txt);
    void WrapData(const QString& text,QLabel* lbl);
    QString SpliteText(const QFontMetrics& font, const QString& text, int nLabelSize,int& index);
	void StartHideAnimation(int dur = 1000);

private:
	Ui::ImageTipsWidgetWidget ui;
	static int curDisplayIndex;
	static int curDisplayMoveIndex;
private slots:
	void OnFinished();
};
