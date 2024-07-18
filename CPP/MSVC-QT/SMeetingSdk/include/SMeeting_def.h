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
		std::string room_no;				//����ţ������ø��˷���Ż�գ��ջ��Զ�����
		std::string title;					//������⣨���
		std::string content;				//����˵�������
		std::string password;				//����
		int meeting_type = 1;				//1��ʱ���� 2ԤԼ����
		long long begin_time = 0;			//ԤԼ��������ʼʱ��unixʱ���
		long long end_time = 0;				//ԤԼ����������ʱ��unixʱ���
		int entry_mute_policy = 3;			//��ᾲ��״̬ Ĭ��3  1:������ᾲ��(���������Ĭ�Ͼ���)  2:�ر�(����ͻ��˳�ʼ��Ƶ״̬) 3����6�˾���(����6�˺���ᾲ��)
		bool watermark_disabled = true;		//ˮӡ�Ƿ�ر� Ĭ��true  false:���� true:�ر�
		bool screenshot_disabled = false;	//�����Ƿ��ֹ Ĭ��false  false:����ֹ true����ֹ
		bool chat_disabled = false;			//�����Ƿ��ֹ Ĭ��false  false:����ֹ true����ֹ
		std::string extend_info;			//��չ
	};

	enum class StatusCode
	{
		//����ϵͳ������
		OK = 0,
		SystemError = 1,			//ϵͳ�ڲ�����
		NotInitialized = 2,			//δ��ʼ��
		MediaNotInitialized = 3,	//ý��ģ����δ��ʼ��
		VideoCapturerError = 100,	//�ⲿ��Ƶ�豸����
		AudioCapturerError = 101,	//�ⲿ��Ƶ�豸����
		AudioPlayerError = 102,     //�ⲿ����������
		DeviceNoRights = 103,			//�豸������Ȩ��
		DeviceNotfound = 104,			//�豸������
		ProtocolParsingError = 200, //Э���������


		//ǰ���ͨ�ô���
		Timeout = 1000,             //��ʱ
		InvalidArgs = 1001,         //��������
		Conflict = 1002,            //�ظ�������ͻ
		NotYetInRoom = 1003,			//��δ���뷿��
		ForbiddenInNetTesting = 1004, //�������ʱ�ܾ�����
		NotFound = 1100,            //���ҵ���Դ������
		UserNotFound = 1101,        //�û�������
		RoomNotFound = 1102,        //���䲻����
		StreamNotFound = 1103,		//����������
		SessionNotFound = 1104,     //�Ự������
		SessionKickout = 1105,      //���߳�
		SdkTokenInvalid = 1106,      //SDK tokenʧЧ

		//���������
		NetError = 2000,            //�������
		MediaNetError = 2001,       //ý���������


		//Ӧ����صĴ���
		InvalidAppId = 3001,        //��Ч��AppID
		ApiSignatureError = 3002,   //ǩ������
		ApiTimestampError = 3003,   //Api����ʱ�������

		//Ȩ�������
		Unauthorized = 4000,        //����δ��Ȩ
		Forbidden = 4001,           //����������
		ConcLimit = 4002,           //��������

		//����˴���
		ServerError = 5000,         //�������ڲ�����

		MQTTServerLess = 5010,      //MQTT��������Դ����
		MQTTServerLineLess = 5011,      //MQTTû�з�������·

		UploadServerLess = 5020,		//��ý���������Դ����
		UploadServerLineLess = 5021,    //��ý��û�з�������·
		UploadWebRtcLess = 5022,		//��ý��������󶨵�WebRTC���ز���
		UploadRtmpLess = 5023,			//��ý��������󶨵�RTMP���ز���

		WebRtcServerLess = 5030,		//WebRTC��������Դ����
		WebRtcServerLineLess = 5031,    //WebRTCû�з�������·

		RTMPServerLess = 5040,          //RTMP��������Դ����
		RTMPServerLineLess = 5041,      //RTMPû�з�������·


		UserNoLogin = 10000,			//�û�δ��¼
		MediaIsProbe = 10001,			//�����������
		DevNoFind = 10002,				//�豸δ�ҵ�
		DevHasOpened = 10003,			//�豸�Ѿ�����
		DevHasClosed = 10004,			//�豸�Ѿ����ر�
		NotSupported = 10005,			//������֧��
		NotDevPrivate = 10006,			//���豸Ȩ��

		//�������Զ������
		ThirdPartyRefused = 100000,     //������ƽ̨��Ȩ����

	};
}