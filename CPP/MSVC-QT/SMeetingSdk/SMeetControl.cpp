#include "SMeetControl.h"
#include <QDebug>
#include "NetWork/HttpNetwork.h"
#include "CJsonObject/JsonUtil.h"

SMeetControl* SMeetControl::_cc = nullptr;
SMeetControl* SMeetControl::Get()
{
    if (!_cc)
    {
        _cc = new SMeetControl();
    }
    return _cc;
}
SMeetControl::SMeetControl()
{
    qDebug()<<__func__<<SdkVersion();
}


SMeeting::StatusCode SMeetControl::InitSDK(QString host,QString logPath)
{
    std::map<std::string, std::string> map;
    map["host"] = host.toStdString();
    if (!logPath.isEmpty()) {
        map["logPath"] = logPath.toStdString();
    }
    SMeeting::StatusCode ret = SMeeting::SMeetingEngine_Init(map, &_imeet);
    if (_imeet) {
        if ((ret == SMeeting::StatusCode::OK))
        {
            _imeet->setEventHandler(this);
        }
        else {
            FreeSdk();
        }
    }

    qDebug() << __func__ << (int)ret;
    return ret;
}

QString SMeetControl::SdkVersion()
{
    qDebug() << __func__;
    std::string v1;
    SMeeting::SMeetingEngine_Version(v1);
    QString ver1 = QString::fromStdString(v1);
    return ver1;
}


void SMeetControl::FreeSdk()
{
    qDebug() << __func__;
    if (_imeet) {
        _imeet->del();
        _imeet = nullptr;
    }
}

QString SMeetControl::GetCodeMsg(int ret)
{
    //mmm
    //char msg[100]={0};
    //RTCEngine_GetStatusMsg((SRTC::StatusCode)ret,msg);
    //return QString::fromStdString(QString::fromLocal8Bit(msg).toStdString());
    return "";
}

SMeeting::StatusCode SMeetControl::LoginUser(QString token)
{
    qDebug()<<__func__<<"token:"<<token;
    SMeeting::StatusCode sc = _imeet->login(token.toStdString(), [&](SMeeting::StatusCode Status, std::string msg) {
            qDebug() << __func__ << "_imeet->login:" << (int)Status;
            emit smtLoginFinish((int)Status);
        });
    qDebug()<<__func__<<"_imeet->login:"<<(int)sc;
    return sc;
}


SMeeting::StatusCode SMeetControl::CreateRoom(QString title)
{
    qDebug()<<__func__<<"title:"<<title;
    SMeeting::SMeetingCreateMeetingModel m;
    m.title = title.toStdString();
    SMeeting::StatusCode sc = _imeet->createRoom(m, [&](SMeeting::StatusCode Status, std::string msg) {
        emit smtCreateFinish((int)Status, QString::fromStdString(msg));
        });
    qDebug()<<__func__<<"_imeet->createRoom:"<<(int)sc;
    return sc;
}

int SMeetControl::SendMsg(QString tar,QString action,QString msg)
{
    //mmm
    return 0;
}

SMeeting::StatusCode SMeetControl::EnterRoom(QString enter,QString name,QString pass)
{
    qDebug()<<__func__<<enter;
    auto sc = _imeet->enterRoom(enter.toStdString(), name.toStdString(), pass.toStdString(), 
        [&](SMeeting::StatusCode Status, std::string msg) {
            emit smtEnterRoomFinish((int)Status,QString::fromStdString(msg));
        });
    return sc;
}
void SMeetControl::Logout(SMeeting::Callback back)
{
    qDebug()<<__func__;
    _imeet->exitRoom(back);
}

SMeeting::StatusCode SMeetControl::SetCameraView(SMeeting::IRTCView* hwnd)
{
    //上文设置了  _imeet->setDisplayView(SMeeting::DisplayEnum::RenderBase);
    //QWidget 是GdiRenderView
    //GdiRenderView 继承了 IRenderBase
    //直接设置对应类对象
    return _imeet->setCameraView(hwnd);
}

SMeeting::StatusCode SMeetControl::SwitchCamera(QString name)
{
    //不设置摄像头名称将直接打开第一个设备
    //如果设备已经打开，切换设备将直接切换对应设备
    return _imeet->switchCamera(name.toStdString());
}

SMeeting::StatusCode SMeetControl::OpenCamera(SMeeting::Callback back)
{
    return _imeet->requestOpenCamera([&](SMeeting::StatusCode Status, std::string msg) {
        smtCameraUpdate(1,(int)Status,QString::fromStdString(msg));
    });
}


bool SMeetControl::IsOpenCamera()
{
    //mmm
    // qDebug() << __func__;
    // if(_camStream)
    // {
    //     return (_camStream->isPlay() == 1);
    // }
    return false;
}
SMeeting::StatusCode SMeetControl::CloseCamera(SMeeting::Callback back)
{
    return _imeet->closeCamera([&](SMeeting::StatusCode Status, std::string msg) {
        smtCameraUpdate(0,(int)Status, QString::fromStdString(msg));
        });
}

SMeeting::StatusCode SMeetControl::SwitchMic(QString name)
{
    //不设置摄像头名称将直接打开第一个设备
    //如果设备已经打开，切换设备将直接切换对应设备
    return _imeet->switchMic(name.toStdString());
}
SMeeting::StatusCode SMeetControl::OpenMic(SMeeting::Callback back)
{
    return _imeet->requestOpenMic([&](SMeeting::StatusCode Status, std::string msg) {
        smtMicUpdate(1, (int)Status, QString::fromStdString(msg));
        });
}
bool SMeetControl::IsOpenMic()
{
    return 0;
}

SMeeting::StatusCode SMeetControl::CloseMic(SMeeting::Callback back)
{
    qDebug()<<__func__;
    return _imeet->closeMic([&](SMeeting::StatusCode Status, std::string msg) {
        smtMicUpdate(0, (int)Status, QString::fromStdString(msg));
        });
}
SMeeting::StatusCode SMeetControl::SwitchSpeaker(QString name)
{
    return _imeet->switchSpeaker(name.toStdString());
}

SMeeting::StatusCode SMeetControl::OpenSpeaker()
{
    return _imeet->openSpeaker();
}

bool SMeetControl::IsOpenSpeaker()
{
    qDebug() << __func__;
    // if(_speakerStream)
    // {
    //     return (_speakerStream->isPlay() == 1);
    // }
    return false;
}
SMeeting::StatusCode SMeetControl::CloseSpeaker()
{
    return _imeet->closeSpeaker();
}

SMeeting::StatusCode SMeetControl::OpenScreen(int hwnd, QRect rect)
{

    qDebug()<<__func__<<rect;
    QString s = QString("%1,%2,%3,%4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
    //1 屏幕，2 白板
    int tp = 1;
    return _imeet->requestShare(tp, s.toStdString(), [&,tp](SMeeting::StatusCode Status, std::string msg) {
        emit smtShareUpdate(tp, (int)Status, QString::fromStdString(msg));
        });
} 

bool SMeetControl::IsOpenScreen()
{
    qDebug() << __func__;
    // if(_screenStream)
    // {
    //     return (_screenStream->isPlay() == 1);
    // }
    return false;
}
SMeeting::StatusCode SMeetControl::CloseScreen()
{
    qDebug()<<__func__;
    return _imeet->stopShare([&](SMeeting::StatusCode Status, std::string msg) {
        smtShareUpdate(0, (int)Status, QString::fromStdString(msg));
        });
}

void SMeetControl::EnumCameraDev(VideoDeviceList& devs)
{
    qDebug() << __func__;
    // char* data;
    // int dataindex;
    // SRTC::StatusCode ret = _irtc->enumVideo(&data,&dataindex);
    // qDebug()<<__func__<<(int)ret<<data;
    // if(dataindex > 0)
    // {
    //     QString json = QString::fromLocal8Bit(data, dataindex);
    //     QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toStdString().c_str());
    //     QJsonObject obj = jsonDocument.object();
    //     devs.Load(obj);
    //     free(data);
    // }
}
void SMeetControl::EnumMicDev(DeviceList& devs)
{
    qDebug() << __func__;
    // char* data;
    // int dataindex;
    // SRTC::StatusCode ret = _irtc->enumAudio(&data,&dataindex);
    // qDebug() << __func__<<(int)ret << data;
    // if(dataindex > 0)
    // {
    //     QString json = QString::fromLocal8Bit(data, dataindex);
    //     QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toStdString().c_str());
    //     QJsonObject obj = jsonDocument.object();
    //     devs.Load(obj);
    //     free(data);
    // }
}
void SMeetControl::EnumSpeakerDev(DeviceList& devs)
{
    qDebug() << __func__;
    // char* data;
    // int dataindex;
    // SRTC::StatusCode ret = _irtc->enumSpeaker(&data,&dataindex);
    // qDebug() << __func__ << (int)ret << data;
    // if(dataindex > 0)
    // {
    //     QString json = QString::fromLocal8Bit(data, dataindex);
    //     QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toStdString().c_str());
    //     QJsonObject obj = jsonDocument.object();
    //     devs.Load(obj);
    //     free(data);
    // }
}
void SMeetControl::EnumScreenDev(ScreenDeviceList& devs)
{
    qDebug() << __func__;
    // char* data;
    // int dataindex;
    // SRTC::StatusCode ret = _irtc->enumScreen(&data,&dataindex);
    // qDebug() << __func__ << (int)ret << data;
    // if(dataindex > 0)
    // {
    //     QString json = QString::fromLocal8Bit(data, dataindex);
    //     QJsonDocument jsonDocument = QJsonDocument::fromJson(json.toStdString().c_str());
    //     QJsonObject obj = jsonDocument.object();
    //     devs.Load(obj);
    //     free(data);
    // }
}

void SMeetControl::LoadRemoteVideoData(QString id,int track,void* hwnd)
{
    qDebug()<<__func__<<id<<track<<hwnd;
    // SRTC::IRTCRemoteStream* rvs = nullptr;
    // _irtc->getMemberStream(id.toLocal8Bit(),&rvs);
    // if(rvs)
    // {
    //     rvs->setView(track,hwnd);
    //     rvs->subscribe();
    //     rvs->play();
    // }
}

void SMeetControl::UnLoadRemoteVideoData(QString id)
{
    qDebug() << __func__ << id;
    // SRTC::IRTCRemoteStream* rvs = nullptr;
    // _irtc->getMemberStream(id.toLocal8Bit(),&rvs);
    // if(rvs)
    // {
    //     rvs->unsubscribe();
    //     rvs->stop();
    // }
}

void SMeetControl::ReflashView(QString id)
{
    qDebug()<<__func__<<id;
    // SRTC::IRTCRemoteStream* rvs = nullptr;
    // _irtc->getMemberStream(id.toLocal8Bit(),&rvs);
    // if(rvs)
    // {
    //     rvs->reflashView();
    // }
}

void SMeetControl::CloseRoom()
{
    qDebug() << __func__;
    _imeet->exitRoom([&](SMeeting::StatusCode Status, std::string msg) {
        emit smtExitRoomFinish((int)Status, QString::fromStdString(msg));
        });
}

int SMeetControl::SendRoomMsg(QString action,QString msg)
{
    qDebug() << __func__ << action << msg;
    //return (int)_irtc->sendRoomMessage(action.toLocal8Bit(),msg.toStdString().c_str(),msg.toStdString().size());
    return 0;
}

void SMeetControl::BeginCustomVideo(int w,int h,int fps,int bit,int trackid)
{
    qDebug() << __func__ ;
    // if(!_customLocalVideoStream)
    // {
    //     SRTC::StatusCode ret = _irtc->createLocalCustomVideoStream(trackid,0,w,h,fps,bit,&_customLocalVideoStream);
    //     if((ret != SRTC::StatusCode::OK) || !_customLocalVideoStream )
    //     {
    //         qDebug()<<__func__<<"error"<<(int)ret;
    //         return ;
    //     }
    // }
    // _customLocalVideoStream->publish();
}
void SMeetControl::PushCustomVideoData(const unsigned char* data, int datasize, int trackId, long long pts,long long dts)
{
    // if(_customLocalVideoStream)
    // {
    //     _customLocalVideoStream->pushVideoData(data,datasize,pts,dts);
    // }
}

void SMeetControl::ProbeNetwork(int time,int up,int down)
{
    qDebug() << __func__;
    // _irtc->probeNetwork(time,up,down);
}
void SMeetControl::UpdateRoomShareProps(bool v)
{
    qDebug() << __func__;
    // RoomPropsData rpd;
    // rpd.set_shareState(v ? SHARESTARE_DESKTOP : SHARESTARE_NONE);
    // rpd.set_uid(sdkInfo.userid);
    // UpdateRoomProps(rpd.toJson());
}

void SMeetControl::UpdateRoomProps(QByteArray byte)
{
    qDebug()<<__func__<<byte;
    // _irtc->updateRoom(byte.data(),byte.size());
}

void SMeetControl::UpdateProps(){
    QByteArray byte = sdkInfo.props.ToJson();

    qDebug() << __func__ << byte;
    // _irtc->updateUser(byte.data(),byte.size());
}

void SMeetControl::UpdateNickName(QString nname)
{
    qDebug() << __func__ << nname;
    _imeet->updateName(nname.toStdString(), [&](SMeeting::StatusCode Status, std::string msg) {
        smtNickNameUpdate((int)Status, QString::fromStdString(msg));
    });
}



//房间内事件回调
void SMeetControl::onRoomNameChanged()
{

}

void SMeetControl::onRoomCameraStateChanged()
{

}

void SMeetControl::onRoomMicStateChanged()
{

}

void SMeetControl::onRoomChatDisabledChanged()
{

}

void SMeetControl::onRoomScreenshotDisabledChanged()
{

}

void SMeetControl::onRoomDestroy()
{

}

void SMeetControl::onRoomKickedOut()
{

}

void SMeetControl::onRoomShareStart()
{

}

void SMeetControl::onRoomShareStop()
{

}

void SMeetControl::onRoomMCUModeChanged()
{

}

void SMeetControl::onRoomHandUpChanged()
{

}

void SMeetControl::onRoomLockChanged()
{

}



//房间内用户事件回调
void SMeetControl::onUserEnter(std::string roomid, std::string user)
{
    qDebug() << __func__;
    emit UserEnter(QString::fromStdString(roomid),QString::fromStdString(user));
}

void SMeetControl::onUserExit(std::string roomid, std::string user)
{
    qDebug() << __func__;
    emit UserExit(QString::fromStdString(roomid),QString::fromStdString(user));
}

void SMeetControl::onUserNameChanged(std::string roomid, std::string)
{
    qDebug() << __func__;
    
}

void SMeetControl::onUserRoleChanged(std::string roomid, int)
{
    qDebug() << __func__;
}

void SMeetControl::onUserCameraStateChanged(std::string roomid, int)
{
    qDebug() << __func__;
}

void SMeetControl::onUserAudioStateChanged(std::string roomid, int)
{
    qDebug() << __func__;
}

void SMeetControl::onUserChatDisabledChanged(std::string roomid, int)
{
    qDebug() << __func__;
}

void SMeetControl::onHandupConfirm()
{
    qDebug() << __func__;
}


//房间内消息事件回调
void SMeetControl::onReceiveChatMessage()
{
    qDebug() << __func__;
}

void SMeetControl::onReceiveCustomMessage()
{
    qDebug() << __func__;
}


//房间内媒体消息事件回调
void SMeetControl::onDeviceChange()
{
    qDebug() << __func__;
}

void SMeetControl::onDefDeviceChange()
{
    qDebug() << __func__;
}


//错误事件回调
void SMeetControl::onError(SMeeting::StatusCode code, std::string s)
{
    qDebug() << __func__ << (int)code << s.c_str();
}

