#pragma once

#include <QWidget>
#include "ui_WidRegister.h"
class QPushButton;
class WidRegister : public QWidget
{
	Q_OBJECT

public:
	WidRegister(QWidget *parent = nullptr);
	~WidRegister();
	void initData();
private slots:
	void on_btnRegister_clicked();
	void on_btnCode_clicked();
	void on_btnBack_clicked();
	void OnPassEyeClicked();
	void OnPassEyeClicked2();
	void OnCodeTimer();
signals:
	void RegisterFinish();
	void RegisterBack();
private:
	Ui::WidRegisterClass ui;
	QPushButton* btn;
	QPushButton* btn2;
	QTimer* codeTimer;
	int codeCount;
};
