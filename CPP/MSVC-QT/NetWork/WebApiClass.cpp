#include "WebApiClass.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUuid>

#pragma execution_character_set("utf-8")

void WebApiClass::ServerUser_AuthGrant(QString name, std::function<void(QByteArray& byte)> function = nullptr)
{
	QJsonDocument doc;
	QJsonObject obj;
	obj["user_id"] = name;
	doc.setObject(obj);
	HttpNetwork::OnPost("/server/user-auth/grant", doc.toJson(QJsonDocument::Compact), function);
}

void WebApiClass::AccountRegister(QString name, QString password, QString nickname, QString mobile, QString avatar, std::function<void(QByteArray& byte)> function)
{
	QJsonDocument doc;
	QJsonObject obj;
	obj["user_name"] = name;
	obj["password"] = password;
	obj["nick_name"] = nickname;
	obj["avatar"] = avatar;
	obj["mobile"] = mobile;
	doc.setObject(obj);

	HttpNetwork::OnPost("account/register", doc.toJson(QJsonDocument::Compact) , function);
}