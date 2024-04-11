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

	static void ServerUser_AuthGrant(QString name , std::function<void(QByteArray& byte)> function = nullptr);

	static void AccountRegister(QString name, QString password, QString nickname, QString mobile, QString avatar, std::function<void(QByteArray& byte)> function = nullptr);

	
};
