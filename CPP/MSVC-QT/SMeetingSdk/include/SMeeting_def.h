#pragma once




#include <functional>
#include <string>
#include <map>
namespace SMeeting {

#define SYSTEM_DEFAULT_MAUDIO_DEVICE "default"
#define REFLASHVIEWCOLOR			 int
#define REFLASHCOLOR(r,g,b)			((((char)r)<<16)|(((char)g)<<8)|(((char)b)<<0))
	enum class ViewEnumType
	{
		Hwnd,
		CallBack
	};

	class IRTCView {
	public:
		virtual ViewEnumType ViewType() = 0;
		virtual void* getHwnd() = 0;
		virtual void updatePlanes(const unsigned char* buf, int w, int h, int fourcc, int label) = 0;
		virtual void updateFull(int r, int g, int b) = 0;
	};

	struct SMeetingCreateMeetingModel
	{
		std::string room_no;				//房间号，可以用个人房间号或空，空会自动生成
		std::string title;					//会议标题（必填）
		std::string content;				//会议说明（必填）
		std::string password;				//密码
		int meeting_type = 1;				//1即时会议 2预约会议
		long long begin_time = 0;			//预约会议必填，开始时间unix时间戳
		long long end_time = 0;				//预约会议必填，结束时间unix时间戳
		int entry_mute_policy = 3;			//入会静音状态 默认3  1:开启入会静音(所有人入会默认静音)  2:关闭(跟随客户端初始音频状态) 3：超6人静音(超过6人后入会静音)
		bool watermark_disabled = true;		//水印是否关闭 默认true  false:开启 true:关闭
		bool screenshot_disabled = false;	//截屏是否禁止 默认false  false:不禁止 true：禁止
		bool chat_disabled = false;			//聊天是否禁止 默认false  false:不禁止 true：禁止
		std::string extend_info;			//扩展
	};

	enum class StatusCode
	{
		//本地系统级错误
		OK = 0,
		SystemError = 1,			//系统内部错误
		NotInitialized = 2,			//未初始化
		MediaNotInitialized = 3,	//媒体模块尚未初始化
		VideoCapturerError = 100,	//外部视频设备错误
		AudioCapturerError = 101,	//外部音频设备错误
		AudioPlayerError = 102,     //外部扬声器错误
		DeviceNoRights = 103,			//设备访问无权限
		DeviceNotfound = 104,			//设备不存在
		ProtocolParsingError = 200, //协议解析错误


		//前后端通用错误
		Timeout = 1000,             //超时
		InvalidArgs = 1001,         //参数错误
		Conflict = 1002,            //重复操作冲突
		NotYetInRoom = 1003,			//尚未加入房间
		ForbiddenInNetTesting = 1004, //网络测试时拒绝操作
		NotFound = 1100,            //查找的资源不存在
		UserNotFound = 1101,        //用户不存在
		RoomNotFound = 1102,        //房间不存在
		StreamNotFound = 1103,		//码流不存在
		SessionNotFound = 1104,     //会话不存在
		SessionKickout = 1105,      //被踢出
		SdkTokenInvalid = 1106,      //SDK token失效

		//网络类错误
		NetError = 2000,            //网络错误
		MediaNetError = 2001,       //媒体网络错误


		//应用相关的错误
		InvalidAppId = 3001,        //无效的AppID
		ApiSignatureError = 3002,   //签名错误
		ApiTimestampError = 3003,   //Api请求时间戳错误

		//权限类错误
		Unauthorized = 4000,        //操作未授权
		Forbidden = 4001,           //操作不允许
		ConcLimit = 4002,           //并发不足

		//服务端错误
		ServerError = 5000,         //服务器内部错误

		MQTTServerLess = 5010,      //MQTT服务器资源不足
		MQTTServerLineLess = 5011,      //MQTT没有服务器线路

		UploadServerLess = 5020,		//流媒体服务器资源不足
		UploadServerLineLess = 5021,    //流媒体没有服务器线路
		UploadWebRtcLess = 5022,		//流媒体服务器绑定的WebRTC网关不足
		UploadRtmpLess = 5023,			//流媒体服务器绑定的RTMP网关不足

		WebRtcServerLess = 5030,		//WebRTC服务器资源不足
		WebRtcServerLineLess = 5031,    //WebRTC没有服务器线路

		RTMPServerLess = 5040,          //RTMP服务器资源不足
		RTMPServerLineLess = 5041,      //RTMP没有服务器线路


		UserNoLogin = 10000,			//用户未登录
		MediaIsProbe = 10001,			//正在质量检测
		DevNoFind = 10002,				//设备未找到
		DevHasOpened = 10003,			//设备已经被打开
		DevHasClosed = 10004,			//设备已经被关闭
		NotSupported = 10005,			//操作不支持
		NotDevPrivate = 10006,			//无设备权限

		//第三方自定义错误
		ThirdPartyRefused = 100000,     //第三方平台鉴权错误

	};
}