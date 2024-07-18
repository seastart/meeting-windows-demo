#pragma once

#include <QObject>
#include <functional>
#include <unordered_map>
#include <QStringList>
#include "HttpNetwork.h"
class WebApiClass : public QObject
{
	Q_OBJECT

public:
	WebApiClass() {};
	~WebApiClass() {};
public:

	static void AuthLogin_Account(QString name , QString pass , std::function<void(QByteArray& byte)> function = nullptr);

	static void AuthRegister(QString mobile, QString password,  QString captcha, std::function<void(QByteArray& byte)> function = nullptr);

	static void AuthSms_Code(QString mobile, std::function<void(QByteArray& byte)> function = nullptr);


	static void Meeting_Grant(std::function<void(QByteArray& byte)> function = nullptr);
};
