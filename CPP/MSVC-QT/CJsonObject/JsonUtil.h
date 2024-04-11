#ifndef JSONUTIL_H
#define JSONUTIL_H


#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QList>
#include <QDebug>
class JsonUtil : public QObject
{
	Q_OBJECT

public:
	static bool GetValue(QJsonObject obj, QString key, QString& value)
	{
		bool ret = false;
		QJsonValue v = obj.value(key);
		if (v != QJsonValue::Undefined)
		{
			value = v.toString();
			ret = true;
		}
		return ret;
	}
	static bool GetValue(QJsonObject obj, QString key, int& value)
	{
		bool ret = false;
		QJsonValue v = obj.value(key);
		if (v != QJsonValue::Undefined)
		{
			value = v.toInt();
			ret = true;
		}
		return ret;
	}
	static bool GetValue(QJsonObject obj, QString key, double& value)
	{
		bool ret = false;
		QJsonValue v = obj.value(key);
		if (v != QJsonValue::Undefined)
		{
			value = v.toDouble();
			ret = true;
		}
		return ret;
	}
	static bool GetValue(QJsonObject obj, QString key, bool& value)
	{
		bool ret = false;
		QJsonValue v = obj.value(key);
		if (v != QJsonValue::Undefined)
		{
			value = v.toBool();
			ret = true;
		}
		return ret;
	}
	static bool GetValue(QJsonObject obj, QString key, QJsonObject& value)
	{
		bool ret = false;
		QJsonValue v = obj.value(key);
		if (v != QJsonValue::Undefined)
		{
			value = v.toObject();
			ret = true;
		}
		return ret;
	}
	static bool GetValue(QJsonObject obj, QString key, std::string& value)
	{
		bool ret = false;
		QJsonValue v = obj.value(key);
		if (v != QJsonValue::Undefined)
		{
			value = v.toString().toStdString();
			ret = true;
		}
		return ret;
	}

	// QString >> QJson
	static QJsonObject GetJsonObjectFromString(const std::string jsonString) {
		QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.c_str());
		if (jsonDocument.isEmpty())
		{
			qDebug() << "jsonDocument is empty" << QString::fromStdString(jsonString);
		}
		QJsonObject jsonObject = jsonDocument.object();
		return jsonObject;
	}


	// QString >> QJsonArray
	static QJsonArray GetJsonArrayFromString(const std::string jsonString) {
		QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.c_str());
		if (jsonDocument.isEmpty())
		{
			qDebug() << "jsonDocument is empty";
		}
		QJsonArray jsonObject = jsonDocument.array();
		return jsonObject;
	}
	// QJson >> QString
	static QString GetStringFromJsonObject(const QJsonObject& jsonObject) {
		return QString(QJsonDocument(jsonObject).toJson());
	}
};


#endif // JSONUTIL_H
