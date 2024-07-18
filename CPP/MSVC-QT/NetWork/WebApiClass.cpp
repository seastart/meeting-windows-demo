#include "WebApiClass.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUuid>

#pragma execution_character_set("utf-8")

void WebApiClass::AuthLogin_Account(QString account,QString password, std::function<void(QByteArray& byte)> function)
{
	QJsonDocument doc;
	QJsonObject obj;
	obj["account"] = account;
	obj["password"] = password;
	doc.setObject(obj);
	HttpNetwork::OnPost("/api/v1/auth/login-account", doc.toJson(QJsonDocument::Compact), function);
}

void WebApiClass::AuthRegister(QString mobile, QString password, QString captcha, std::function<void(QByteArray& byte)> function)
{
	QJsonDocument doc;
	QJsonObject obj;
	obj["password"] = password;
	obj["captcha"] = captcha;
	obj["mobile"] = mobile;
	doc.setObject(obj);

	HttpNetwork::OnPost("/api/v1/auth/register", doc.toJson(QJsonDocument::Compact) , function);
}

void WebApiClass::AuthSms_Code(QString mobile, std::function<void(QByteArray& byte)> function)
{

	QJsonDocument doc;
	QJsonObject obj;
	obj["scene"] = "register";
	obj["mobile"] = mobile;
	doc.setObject(obj);

	HttpNetwork::OnPost("/api/v1/auth/sms-code", doc.toJson(QJsonDocument::Compact), function);
}


void WebApiClass::Meeting_Grant(std::function<void(QByteArray& byte)> function)
{
	HttpNetwork::OnGet("/api/v1/meeting/grant", function);
}