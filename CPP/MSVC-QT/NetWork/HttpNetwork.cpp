#include "HttpNetwork.h"
#include "Tools/Utils.h"
#include "CJsonObject/JsonUtil.h"

#pragma execution_character_set("utf-8")
HttpNetwork* HttpNetwork::object = nullptr;

HttpNetwork::HttpNetwork()
{
    host = "http://192.168.0.172:5000/test/";
	network = new QNetworkAccessManager(this);
	QObject::connect(network, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnRequestComplete(QNetworkReply*)));
}

HttpNetwork* HttpNetwork::Get()
{
	if (HttpNetwork::object == nullptr) {
		HttpNetwork::object = new HttpNetwork();
	}
	return HttpNetwork::object;
}

void HttpNetwork::SetHost(QString inHost)
{
	host = inHost;
}
	
// 获取地址
QString HttpNetwork::Host()
{
	return host;
}

void HttpNetwork::SetToken(QString tk)
{
	token = tk;
}
QString HttpNetwork::Token()
{
	return token;
}

void HttpNetwork::OnDownload(const char* url, std::function<void(QByteArray& byte)> function)
{
	qDebug() << __func__ << url;
	QNetworkRequest request = QNetworkRequest(QUrl(url));

	QSslConfiguration config = request.sslConfiguration();
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::TlsV1SslV3);
	request.setSslConfiguration(config);

	//mm这里还有一个问题，那就是，如果连续一个人同时下载头像2次的话，前面这个下载的会被冲掉。但是程序没问题。
	// 保存回调函数
    HttpNetwork::Get()->callMap.insert(request.url().toString() , function);

	// 发送请求
	HttpNetwork::Get()->network->get(request);
}

void HttpNetwork::OnGet(
	const char* url,
	std::function<void(QByteArray& byte)> function)
{
	QString requestUrl;


	requestUrl = Get()->Host() + url;
	qDebug() << __func__ << "on post url:" << requestUrl;
	if (HttpNetwork::Get()->callMap.contains(requestUrl))
	{
		QString jsonNetError = tr("{\"code\":1714,\"msg\":\"操作被取消\"}");
		QByteArray byte = jsonNetError.toStdString().c_str();
		HttpNetwork::Get()->callMap[requestUrl](byte);
		//return;
	}

	QNetworkRequest request = Get()->CreateRequest(requestUrl);
	request.setPriority(QNetworkRequest::Priority::HighPriority);

	// 保存回调函数
	HttpNetwork::Get()->callMap[request.url().toString()] = function;
	// 发送请求s
	QNetworkReply* reply = HttpNetwork::Get()->network->get(request);
}
void HttpNetwork::OnPost(
	const char* url,
	QByteArray post_data,
	std::function<void(QByteArray& byte)> function)
{
	QString requestUrl;


	requestUrl = Get()->Host() + url;
    qDebug()<<__func__<<"on post url:" << requestUrl<<"data:"<<post_data;
	if (HttpNetwork::Get()->callMap.contains(requestUrl))
	{
		QString jsonNetError = tr("{\"code\":1714,\"msg\":\"操作被取消\"}");
		QByteArray byte = jsonNetError.toStdString().c_str();
		HttpNetwork::Get()->callMap[requestUrl](byte);
		//return;
	}
	
	QNetworkRequest request = Get()->CreateRequest(requestUrl);
	request.setPriority(QNetworkRequest::Priority::HighPriority);

	// 保存回调函数
	HttpNetwork::Get()->callMap[request.url().toString()] = function;
    // 发送请求s
	QNetworkReply* reply = HttpNetwork::Get()->network->post(request, post_data);
}



void HttpNetwork::ClearUrlPost(QString url)
{
	if (HttpNetwork::Get()->callMap.keys().contains(url))
	{
		HttpNetwork::Get()->callMap.remove(url);
	}
}

QNetworkRequest HttpNetwork::CreateRequest(QString urlString)
{
	QNetworkRequest request = QNetworkRequest(QUrl(urlString));

	QSslConfiguration config = request.sslConfiguration();
	config.setPeerVerifyMode(QSslSocket::VerifyNone);
	config.setProtocol(QSsl::AnyProtocol);
	request.setSslConfiguration(config);


	if (!token.isEmpty()) {
		request.setRawHeader("Authorization", ("Bearer " + token).toLocal8Bit());
	}
    
    request.setRawHeader("Content-Type", "application/json");

	return request;
}

void HttpNetwork::OnRequestComplete(QNetworkReply* reply)
{
	QString key = reply->request().url().toString();
	if (!HttpNetwork::Get()->callMap.contains(key))
	{
		return;
	}



	QByteArray byte = reply->readAll();
    qDebug()<<("url:" + key + "  ,requestComplete:" + byte);
	if (byte.isEmpty())
	{
		QString jsonNetError = tr("{\"code\":1713,\"msg\":\"没有连接上网络\"}");
		byte = jsonNetError.toStdString().c_str();
	}
	std::function<void(QByteArray& byte)> fun = HttpNetwork::Get()->callMap[key];
	if (fun)
	{
		fun(byte);
	}



	HttpNetwork::Get()->callMap.remove(key);
}
