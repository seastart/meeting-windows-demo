#pragma once

#include <QWidget>
#include <QString>
class IChatBase : public QWidget
{
	Q_OBJECT

public:
    IChatBase(QWidget* parent) {}
    ~IChatBase() {}
	virtual void SetData(QString id, QString Name, QString portrait, QString message, bool isShowHead) = 0;
	virtual void SetNickName(QString Name) = 0;

	virtual void SetKey(QString) = 0;
	virtual QString GetKey() = 0;

};
