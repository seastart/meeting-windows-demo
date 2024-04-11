#ifndef HTTPNETWORK_H
#define HTTPNETWORK_H

#include <QNetworkAccessManager>
#include <QObject>
#include <unordered_map>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <QDateTime>
#include "../CJsonObject/JsonUtil.h"

class HttpNetwork : public QObject
{
	Q_OBJECT

public:
	HttpNetwork();

	static HttpNetwork* Get();

	// 设置请求地址
	void SetHost(QString inHost);

	// 获取地址
	QString Host();

	// 下载文件
	static void OnDownload(
		const char* url,
		std::function<void(QByteArray& byte)> function);

	static void OnPost(
		const char* url,
		QByteArray value,
		std::function<void(QByteArray& byte)> function);

	static void ClearUrlPost(QString url);
private slots:
	void OnRequestComplete(QNetworkReply* reply);

private:
	static HttpNetwork* object;
	QNetworkAccessManager* network;
	// 请求地址
	QString host;

	// 创建request
    QNetworkRequest CreateRequest(QString urlString);

	// 回调方法集合
    QMap<QString, std::function<void(QByteArray& byte)>> callMap;
};

#endif // HTTPNETWORK_H
