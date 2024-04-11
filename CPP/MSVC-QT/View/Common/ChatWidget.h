#pragma once

#include <QWidget>
#include "ui_ChatWidget.h"
#include <QTimer>
class ChatWidget : public QWidget
{
	Q_OBJECT

public:
    ChatWidget(QWidget* parent = Q_NULLPTR);
    ~ChatWidget();

    QWidget* GetViewWidget();
    void SetViewLayout(QLayout* l);

public slots:
	void on_btnClose_clicked();

    void OnbtnSendClicked();
protected:
	bool eventFilter(QObject* obj, QEvent* event);
signals:
    void SendMsg(QString tar,QString msg);
private:
    Ui::ChatWidget ui;
};
