#pragma once

#include <QObject>
#include <QPushButton>
#include <QLabel>
#include <functional>

class SetImageClass : public QObject
{
	Q_OBJECT
public:
	//设置头像的图片 按钮控件，
    static void SetPortraitImage(QPushButton* btn, QString ID, QString url, QString def);

	//设置头像的图片 文本控件，
    static void SetPortraitImage(QLabel* btn, QString ID, QString url, QString def);

	static void StopPortraitImageDownLoad(QString url);


	//圆形头像
	static bool LoadPixmapType(QString path, QPixmap& pixmap);
	static bool LoadDefPixmap(QLabel* lbl, QString pix);
	static bool LoadDefPixmap(QLabel* lbl, QPixmap* pixmap);
	static bool LoadDefPixmap(QPushButton* lbl, QString pix);
	static bool LoadDefPixmap(QPushButton* lbl, QPixmap* pixmap);
	static bool LoadPixmap(QLabel* lbl, QPixmap* pix);
	static bool LoadPixmap(QPushButton* lbl, QPixmap* pix);

	static QPixmap PixmapToRound(QWidget* wid, QPixmap pix);
};
