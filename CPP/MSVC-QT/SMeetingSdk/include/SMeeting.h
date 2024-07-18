#pragma once

#if defined OS_EMBEDDED
#define SMEETING_CALL
#else
#define SMEETING_CALL __cdecl
#endif

#if defined OS_ANDROID||defined OS_IOS||defined OS_LINUX||defined OS_EMBEDDED
#define SMEETING_API 
#else
#ifdef SMEETING_DLL
#define SMEETING_API __declspec(dllexport)
#else
#define SMEETING_API __declspec(dllimport)
#endif
#endif
#include "SMeeting_def.h"


namespace SMeeting {

	typedef std::function<void(StatusCode Status, std::string msg)> Callback;


	class ISMeetingBase {
	public:
		virtual void del() = 0;
	};

	class ISMeetingEngineEvent
	{
	public:
		//房间内事件回调
		virtual void onRoomNameChanged() = 0;
		virtual void onRoomCameraStateChanged() = 0;
		virtual void onRoomMicStateChanged() = 0;
		virtual void onRoomChatDisabledChanged() = 0;
		virtual void onRoomScreenshotDisabledChanged() = 0;
		virtual void onRoomDestroy() = 0;
		virtual void onRoomKickedOut() = 0;
		virtual void onRoomShareStart() = 0;
		virtual void onRoomShareStop() = 0;
		virtual void onRoomMCUModeChanged() = 0;
		virtual void onRoomHandUpChanged() = 0;
		virtual void onRoomLockChanged() = 0;


		//房间内用户事件回调
		virtual void onUserEnter(std::string roomid, std::string userdata) = 0;
		virtual void onUserExit(std::string roomid, std::string userdata) = 0;
		virtual void onUserNameChanged(std::string id, std::string newname) = 0;
		virtual void onUserRoleChanged(std::string id, int newrole) = 0;
		virtual void onUserCameraStateChanged(std::string id, int newstate) = 0;
		virtual void onUserAudioStateChanged(std::string id, int newstate) = 0;
		virtual void onUserChatDisabledChanged(std::string id, int newstate) = 0;
		virtual void onHandupConfirm() = 0;

		//房间内消息事件回调
		virtual void onReceiveChatMessage() = 0;
		virtual void onReceiveCustomMessage() = 0;

		//房间内媒体消息事件回调
		virtual void onDeviceChange() = 0;
		virtual void onDefDeviceChange() = 0;

		//错误事件回调
		virtual void onError(SMeeting::StatusCode, std::string) = 0;
	};



	class ISMeetingEngine :public ISMeetingBase
	{
	public:

		//登录登出和事件回调
		virtual StatusCode login(std::string token, Callback back = NULL) = 0;
		virtual StatusCode logout(Callback back = NULL) = 0;
		virtual StatusCode setEventHandler(ISMeetingEngineEvent* e) = 0;
		virtual StatusCode getSelf(Callback back) = 0;

		//房间相关主动接口
		virtual StatusCode createRoom(SMeetingCreateMeetingModel model, Callback back = NULL) = 0;
		virtual StatusCode enterRoom(std::string roomno, std::string name, std::string pass = "", Callback back = NULL) = 0;
		virtual StatusCode exitRoom(Callback back = NULL) = 0;


		virtual StatusCode adminDestroyRoom(std::string roomno, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateRoomName() = 0;
		virtual StatusCode adminUpdateRoomVideoState() = 0;
		virtual StatusCode adminUpdateRoomAudioState() = 0;
		virtual StatusCode adminUpdateRoomCameraState(int self_unmute_camera_disabled, int camera_disabled, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateRoomMicState(int self_unmute_mic_disabled, int mic_disabled, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateRoomChatDisabled(int chat_disabled, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateRoomScreenshotDisabled(int screenshot_disabled, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateRoomMCUMode() = 0;
		virtual StatusCode adminUpdateRoomLocked(int locked, Callback back = NULL) = 0;
		virtual StatusCode adminStopRoomShare(Callback back = NULL) = 0;
		virtual StatusCode adminUpdateUserName(std::string id, std::string name, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateUserRole(std::string id, std::string role, Callback back = NULL) = 0;
		virtual StatusCode adminUpdateUserChatDisabled(std::string id, bool chat_disabled, Callback back = NULL) = 0;
		virtual StatusCode adminDisableUserCamera() = 0;
		virtual StatusCode adminDisableUserMic() = 0;
		virtual StatusCode adminCloseUserCamera(std::string id, Callback back = NULL) = 0;
		virtual StatusCode adminCloseUserMic(std::string id, Callback back = NULL) = 0;
		virtual StatusCode adminRequestUserOpenCamera(std::string id, Callback back = NULL) = 0;
		virtual StatusCode adminRequestUserOpenMic(std::string id, Callback back = NULL) = 0;
		virtual StatusCode adminKickUserOut(std::string id, Callback back = NULL) = 0;
		virtual StatusCode adminConfirmHandup(std::string id, bool approve, Callback back = NULL) = 0;

		//用户相关主动接口
		virtual StatusCode updateName(std::string name, Callback back = NULL) = 0;
		virtual StatusCode requestOpenCamera(Callback back = NULL) = 0;
		virtual StatusCode requestOpenMic(Callback back = NULL) = 0;
		virtual StatusCode requestShare(int tp, std::string data, Callback back = NULL) = 0;
		virtual StatusCode closeCamera(Callback back = NULL) = 0;
		virtual StatusCode closeMic(Callback back = NULL) = 0;
		virtual StatusCode stopShare(Callback back = NULL) = 0;
		virtual StatusCode sendRoomChatMessage(std::string msg, Callback back = NULL) = 0;
		virtual StatusCode sendRoomCustomMessage(std::string, std::string, Callback back = NULL) = 0;
		virtual StatusCode requestHandup(int code, Callback back = NULL) = 0;
		virtual StatusCode cancelHandup(int code, Callback back = NULL) = 0;

		virtual StatusCode setCameraView(SMeeting::IRTCView* view) = 0;
		virtual StatusCode switchCamera(std::string name) = 0;
		virtual StatusCode switchMic(std::string name) = 0;
		virtual StatusCode openSpeaker() = 0;
		virtual StatusCode closeSpeaker() = 0;
		virtual StatusCode switchSpeaker(std::string name) = 0;
		virtual StatusCode loadRemoteVideoData(std::string, std::string trackid, SMeeting::IRTCView* view) = 0;
		virtual StatusCode unLoadRemoteVideoData(std::string, std::string trackid) = 0;

	};

	SMEETING_API StatusCode SMEETING_CALL SMeetingEngine_Init(std::map<std::string, std::string> map, ISMeetingEngine** rtc);
	SMEETING_API StatusCode SMEETING_CALL SMeetingEngine_Version(std::string& s);


};
