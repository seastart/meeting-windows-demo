#ifndef SRTCCONTROL_H
#define SRTCCONTROL_H

#include <QString>
#include <SMeeting.h>
#include <QStandardPaths>
#include <QApplication>
#include <QDir>
#include "VideoDevice.h"
#include "../DataModel/MemberDataModel.h"
#include "../DataModel/RoomDataModel.h"
class SdkInfo
{
public:   
    int customStream = 0;
    QString userid;
    QString userName;
    QString userPortrait;
    QString roomid;
    MemberPropsDataModel props;
};
class GdiRenderView;
class SMeetControl :public QObject,public SMeeting::ISMeetingEngineEvent
{
    Q_OBJECT
public:
    static SMeetControl* Get();
public:
    SMeetControl();
    SMeeting::StatusCode InitSDK(QString host,QString logpath = "");
    void FreeSdk();
    QString SdkVersion();
    SdkInfo sdkInfo;
    QString GetCodeMsg(int);
    SMeeting::StatusCode LoginUser(QString token);
    SMeeting::StatusCode CreateRoom(QString title);



    int SendMsg(QString tar,QString action,QString msg);
    SMeeting::StatusCode EnterRoom(QString enter,QString name,QString pass = "");


    void Logout(SMeeting::Callback back = nullptr);

    SMeeting::StatusCode SetCameraView(SMeeting::IRTCView* hwnd);
    SMeeting::StatusCode SwitchCamera(QString name);
    SMeeting::StatusCode OpenCamera(SMeeting::Callback back = nullptr);
    bool IsOpenCamera();
    SMeeting::StatusCode CloseCamera(SMeeting::Callback back = nullptr);

    SMeeting::StatusCode SwitchMic(QString name);
    SMeeting::StatusCode OpenMic(SMeeting::Callback back = nullptr);
    bool IsOpenMic();
    SMeeting::StatusCode CloseMic(SMeeting::Callback back = nullptr);

    SMeeting::StatusCode SwitchSpeaker(QString name);
    SMeeting::StatusCode OpenSpeaker();
    bool IsOpenSpeaker();
    SMeeting::StatusCode CloseSpeaker();

    SMeeting::StatusCode OpenScreen(int hwnd, QRect rect);
    bool IsOpenScreen();
    SMeeting::StatusCode CloseScreen();

    void EnumCameraDev(VideoDeviceList&);
    void EnumMicDev(DeviceList&);
    void EnumSpeakerDev(DeviceList&);
    void EnumScreenDev(ScreenDeviceList&);

    void LoadRemoteVideoData(QString id,int track,void*hwnd);
    void UnLoadRemoteVideoData(QString id);
    void ReflashView(QString id);

    void CloseRoom();
    int SendRoomMsg(QString action,QString msg);

    void BeginCustomVideo(int w,int h,int fps,int bit,int);
    void PushCustomVideoData(const unsigned char* data, int datasize, int trackId, long long pts,long long dts);



    void ProbeNetwork(int time,int up,int down);

    void UpdateRoomShareProps(bool v);
    void UpdateRoomProps(QByteArray byte);

    void UpdateProps();
    void UpdateNickName(QString nname);

public:
    //房间内事件回调
    void onRoomNameChanged();
    void onRoomCameraStateChanged();
    void onRoomMicStateChanged();
    void onRoomChatDisabledChanged();
    void onRoomScreenshotDisabledChanged();
    void onRoomDestroy();
    void onRoomKickedOut();
    void onRoomShareStart();
    void onRoomShareStop();
    void onRoomMCUModeChanged();
    void onRoomHandUpChanged();
    void onRoomLockChanged();


    //房间内用户事件回调
    void onUserEnter(std::string roomid, std::string user);
    void onUserExit(std::string roomid, std::string user);
    void onUserNameChanged(std::string id, std::string newname);
    void onUserRoleChanged(std::string id, int newrole);
    void onUserCameraStateChanged(std::string id, int newstate);
    void onUserAudioStateChanged(std::string id, int newstate);
    void onUserChatDisabledChanged(std::string id, int newstate);
    void onHandupConfirm();

    //房间内消息事件回调
    void onReceiveChatMessage();
    void onReceiveCustomMessage();

    //房间内媒体消息事件回调
    void onDeviceChange();
    void onDefDeviceChange();

    //错误事件回调
    void onError(SMeeting::StatusCode, std::string);
signals:
    void smtLoginFinish(int);
    void smtCreateFinish(int, QString);
    void smtEnterRoomFinish(int, QString);
    void smtCameraUpdate(int, int, QString);
    void smtMicUpdate(int, int, QString);
    void smtNickNameUpdate(int,QString);
    void smtShareUpdate(int,int, QString);
    void smtExitRoomFinish(int,QString);

    void UserEnter(QString,QString);
    void UserExit(QString,QString);
    void UserNameUpdate(QString uid, QString dt);
    void UserMicUpdate(QString uid, int dt);
    void UserCameraUpdate(QString uid, int dt);
private:

private:
    static SMeetControl* _cc;
    SMeeting::ISMeetingEngine* _imeet;
};

#endif // SRTCCONTROL_H
