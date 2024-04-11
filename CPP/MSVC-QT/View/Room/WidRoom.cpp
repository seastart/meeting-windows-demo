#include "WidRoom.h"
#include <QTimer>
#include <QDebug>
#include "ui_WidRoom.h"
#include "../../Tools/FlowLayout.h"
#include "WidDiaplayView.h"
#include "../../CJsonObject/JsonUtil.h"
#include "../Common/ChatControl.h"
#include "../Common/WidDialog.h"
#include "DataModel/RoomDataModel.h"
#include "Tools/Utils.h"
#include "WidSetData.h"
#include "../Common/ImageTipsWidgetWidget.h"
#include "Global/GlobalDataClass.h"
#include "windows.h"
#include <iostream>
#include <thread>

#pragma execution_character_set("utf-8")
WidRoom::WidRoom(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidRoom)
{
    ui->setupUi(this);
    qDebug() << __func__;
    FlowLayout* fl = new FlowLayout(ui->widViews,0,10,10);
    ui->widViews->setLayout(fl);
    ui->widViews->setStyleSheet("QWidget#widViews{background:#363B41;}");

    connect(SRTCControl::Get(),SIGNAL(UpLevel(int)),this,SLOT(OnUpLevel(int)));
    connect(SRTCControl::Get(),SIGNAL(DownLevel(QString,int)),this,SLOT(OnDownLevel(QString,int)));
    connect(SRTCControl::Get(),SIGNAL(UpStat(QString)),this,SLOT(OnUpStat(QString)));

    connect(SRTCControl::Get(),SIGNAL(DownStat(QString)),this,SLOT(OnDownStat(QString)));
    connect(SRTCControl::Get(),SIGNAL(Speakers(QString)),this,SLOT(OnSpeakers(QString)));
    connect(SRTCControl::Get(),SIGNAL(DevRrecovery(int)),this,SLOT(OnDevRrecovery(int)));
    connect(SRTCControl::Get(),SIGNAL(DevChange(int,int,QString)),this,SLOT(OnDevChange(int,int,QString)));
    connect(SRTCControl::Get(),SIGNAL(DefDevChange(int,int,QString)),this,SLOT(OnDefDevChange(int,int,QString)));

    connect(SRTCControl::Get(),SIGNAL(MemberUserUpdate(QString)),this,SLOT(OnMemberUserUpdate(QString)));
    connect(SRTCControl::Get(),SIGNAL(MemberUserLeave(QString)),this,SLOT(OnMemberUserLeave(QString)));
    connect(SRTCControl::Get(),SIGNAL(RoomUpdate(QString)),this,SLOT(OnRoomUpdate(QString)));

    connect(ui->scrollArea,SIGNAL(WheelChange()),this,SLOT(OnWheelChange()));

    timetimer = new QTimer(this);
    connect(timetimer,&QTimer::timeout,[&]{
        int mt = begindt.msecsTo(QDateTime::currentDateTime());
        ui->lblTime->setText(QTime::fromMSecsSinceStartOfDay(mt).toString());
        ui->lblcpu->setText(QString("%1%").arg(Utils::app_cpu()));
        ui->lblmem->setText(QString("%1M").arg(Utils::GetMemoryUse()));
    });
    ChatControl::Get();




    myView = nullptr;
    reloadLeaveMap.clear();
    SetThreadExecutionState(ES_CONTINUOUS | ES_DISPLAY_REQUIRED | ES_SYSTEM_REQUIRED);
    connect(SRTCControl::Get(),SIGNAL(JoinRoomFinish(QString,QString,QString,QString)),this,SLOT(OnJoinRoomFinish(QString,QString,QString,QString)));
    viewindex = 0;
}

WidRoom::~WidRoom()
{
    qDebug() << __func__;
    delete ui;
}

void WidRoom::appendMsg(QString txt)
{
    qDebug()<<__func__<<txt;
    //ui->txtmsg->append(txt);
}

void WidRoom::OnJoinRoomFinish(QString room,QString me,QString members,QString opts)
{
    needCloseRoom = false;
    qDebug() << __func__ << room << members << opts;
    RoomDataModel rm;
    rm.Load(JsonUtil::GetJsonObjectFromString(room.toStdString()));
    SRTCControl::Get()->sdkInfo.roomid = rm.room_id();

    MemberDataModel lum;
    lum.Load(JsonUtil::GetJsonObjectFromString(me.toStdString()));
    SRTCControl::Get()->sdkInfo.userid = lum.uid();

    QList<QString> oldmem = memberMap.keys();
    QList<QString> newmem;

    QJsonArray arr = JsonUtil::GetJsonArrayFromString(members.toStdString());
    for (QJsonArray::iterator it = arr.begin(); it != arr.end(); ++it)
    {
        QJsonObject a = (*it).toObject();
        MemberDataModel s;
        s.Load(a);
        if(s.uid() == SRTCControl::Get()->sdkInfo.userid){
            continue;
        }
        MemberUpdate(s,false);
        newmem.append(s.uid());
    }

    for(auto s : oldmem){
        if(!newmem.contains(s)){
            qDebug()<<__func__<<"re joinroom leave member:" + s;
            reloadLeaveMap.append(s);
        }
    }


    emit JoinFinish();
}

void WidRoom::InitData(bool cam,bool mic,bool speaker)
{

    qDebug() << __func__;
    QTimer::singleShot(0,[&,cam,mic,speaker]{
        qDebug() <<"InitData" << __func__;
        ImageTipsWidgetWidget::DisplayDialog(this,tr("加入房间成功"));
        if(!myView){
            ui->btnChat->setImgText(tr("聊天"),":/Images/bchat.png");
            ui->btnCamera->setImgText(tr("开启视频"),":/Images/camc.png");
            ui->btnMic->setImgText(tr("解除静音"),":/Images/micc.png");
            ui->btnScreen->setImgText(tr("共享屏幕"),":/Images/bshare.png");

            QString roomid = SRTCControl::Get()->sdkInfo.roomid;
            roomid = QFontMetrics(ui->labTitle->font()).elidedText(roomid, Qt::ElideRight, 500);

            ui->labTitle->setText(roomid);
            myView = CreateMemberView(SRTCControl::Get()->sdkInfo.userid,SRTCControl::Get()->sdkInfo.userName,SRTCControl::Get()->sdkInfo.userPortrait);
            myView->SetToolName(SRTCControl::Get()->sdkInfo.userName+tr(" (我)"));
            begindt = QDateTime::currentDateTime();
            timetimer->start(1000);
            qDebug()<<"InitData"<<"OpenCloseCamera"<<cam;
            OpenCloseCamera(cam);
            qDebug()<<"InitData"<<"OpenCloseMic"<<mic;
            OpenCloseMic(mic);
            qDebug()<<"InitData"<<"OpenCloseSpeaker"<<speaker;
            OpenCloseSpeaker(speaker);
        }

        qDebug() << "InitData" << __func__ << 1;
        //遍历减少的uid,进行删除
        //添加view 没有的ui
        for(auto l : reloadLeaveMap){
            MemberLeave(l);
        }
        reloadLeaveMap.clear();

        qDebug() << "InitData" << __func__ << 2;
        auto it = memberMap.begin();
        for(;it != memberMap.end() ; it++){
            if(!it.value()->memview){
                qDebug()<<"QTimer::singleShot(0,[&,cam,mic,speaker]{"<<it.key();
                WidDiaplayView* l = CreateMemberView(it.key(),it.value()->data.name(),it.value()->data.avatar());
                l->SetToolMic(it.value()->data.props().audioState());
                l->SetToolCam(it.value()->data.props().videoState());
                it.value()->memview = l;
            }
        }
        qDebug() << "InitData" << __func__ << 3;
    });

}


void WidRoom::MemberLeave(QString id)
{
    qDebug() << __func__ << id;
    if(memberMap.contains(id))
    {
        if(id == chkmember){
            GlobalDataClass::Get()->DisplayDialogClose();
        }
        ui->widViews->layout()->removeWidget(memberMap[id]->memview);
        memberMap[id]->memview->deleteLater();
        delete memberMap[id];
        memberMap.remove(id);
        viewindex--;
        if(viewindex == 1){
            myView->setFixedSize(ui->widViewBase->width(),ui->widViewBase->height());
        }
    }
}

bool WidRoom::MemberUpdate(MemberDataModel & mmd,bool updateview)
{
    qDebug() << __func__;
    bool isadd = false;
    if(memberMap.contains(mmd.uid()))
    {
        bool v,a,n;
        CompapreMemberData(mmd.uid(),mmd,a,v,n);
        qDebug()<<__func__<<"MergeData"<<a<<v<<n;
        if(memberMap[mmd.uid()]->memview){
            if(a){
                memberMap[mmd.uid()]->memview->SetToolMic(mmd.props().audioState());
            }
            if(v){
                memberMap[mmd.uid()]->memview->SetToolCam(mmd.props().videoState());
                if(!mmd.props().videoState()){
                    memberMap[mmd.uid()]->memview->SetShowView(mmd.props().videoState());
                    int curTrack = memberMap[mmd.uid()]->memview->GetTrack();
                    if(curTrack != -1){
                        SRTCControl::Get()->UnLoadRemoteVideoData(mmd.uid());
                        memberMap[mmd.uid()]->memview->SetTrack(-1);
                    }
                }
            }
            if(n){
                memberMap[mmd.uid()]->memview->SetToolName(mmd.name());
                ChatControl::Get()->UpdateNickName(mmd.uid(),mmd.name());
            }
        }
        if(n){
            ChatControl::Get()->UpdateNickName(mmd.uid(),mmd.name());
        }
        memberMap[mmd.uid()]->data.MergeData(mmd);
        isadd = false;
    }else
    {
        qDebug()<<__func__<<"append member:"+mmd.uid()+",name:"+mmd.name()+"view:"+QString::number(updateview);
        MemberInfo* mem = new MemberInfo();
        mem->data = mmd;
        if(updateview){
            WidDiaplayView* l = CreateMemberView(mmd.uid(),mmd.name(),mmd.avatar());
            l->SetToolMic(mmd.props().audioState());
            l->SetToolCam(mmd.props().videoState());
            mem->memview = l;
        }else{
            mem->memview = nullptr;
        }

        memberMap.insert(mmd.uid(),mem);
        isadd = true;
    }
    qDebug() << __func__<<"finish";
    return isadd;
}

void WidRoom::CompapreMemberData(QString id,MemberDataModel& n,bool& ac,bool& vc,bool& nc)
{
    nc = vc = ac = false;
    if(memberMap.contains(id)){
        MemberDataModel& o = memberMap[id]->data;
        if(o.props().audioState() != n.props().audioState()){
            ac = true;
        }
        if(o.props().videoState() != n.props().videoState()){
            vc = true;
        }
        if(o.name() != n.name()){
            nc = true;
        }
    }else{
        nc = vc = ac = true;
    }
}


WidDiaplayView* WidRoom::CreateMemberView(QString id,QString name,QString portrait)
{
    qDebug()<<__func__<<id<<name;
    WidDiaplayView* v = new WidDiaplayView(ui->widViews);
    v->SetToolName(name);
    v->SetToolMic(0);
    v->SetToolCam(0);
    v->SetKey(id);
    v->SetToolPortrait(portrait);
    ui->widViews->layout()->addWidget(v);
    if(viewindex == 0){
        v->setFixedSize(ui->widViewBase->width(),ui->widViewBase->height());
    }else if(viewindex == 1){
        myView->setFixedSize((ui->widViewBase->width() - 12)/2 - 4,(ui->widViewBase->height() - 12)/2);
        v->setFixedSize((ui->widViewBase->width() - 12) /2 -4,(ui->widViewBase->height() - 12)/2);
    }else{
        v->setFixedSize((ui->widViewBase->width() - 12) /2 -4,(ui->widViewBase->height() - 12)/2);
    }

    connect(v,SIGNAL(Clicked()),this,SLOT(OnMemberClicked()));

    qDebug()<<__func__<<v->size()<<ui->widViewBase->size();
    if(myView){
        qDebug()<<myView->size();
    }
    viewindex ++;
    return v;
}


bool WidRoom::OpenCloseCamera(bool open,QString name)
{
    qDebug()<<__func__<<open;
    QWidget* view = myView->GetShowViewWidget();
    if(open)
    {
        int ret = SRTCControl::Get()->OpenCamera((void*)view->winId(),name);
        if(!ret)
        {
            ui->btnCamera->setImgText(tr("关闭视频"),":/Images/cam.png");
        }
        else if(ret == (int)SRTC::StatusCode::Unauthorized){
            GlobalDataClass::Get()->DisplayDialogLabel(this,tr("允许使用摄像头"),tr("请在“设置-隐私”下的“相机”中允许应用使用相机"),"",[&](QString s){
                system("Explorer.EXE ms-settings:privacy-webcam");
                return true;
            },tr("前往设置"),tr("取消"));
            return false;
        }else
        {
            ImageTipsWidgetWidget::DisplayDialog(this,tr("摄像头打开异常%1,msg：%2").arg(ret).arg(SRTCControl::Get()->GetCodeMsg(ret)));
            return false;
        }
    }else
    {
        SRTCControl::Get()->CloseCamera();
        ui->btnCamera->setImgText(tr("开启视频"),":/Images/camc.png");
    }
    myView->SetToolCam(open);
    myView->SetShowView(open);
    vState = open;

    qDebug() << __func__ << "finish";
    return true;
}

bool WidRoom::OpenCloseMic(bool open,QString name)
{
    qDebug()<<__func__<<open;
    if(open)
    {
        int ret = SRTCControl::Get()->OpenMic(name);
        if(!ret){
            selectDefMic = name.isEmpty();
            ui->btnMic->setImgText(tr("静音"),":/Images/mic.png");
        }
        else if(ret == (int)SRTC::StatusCode::Unauthorized){
            GlobalDataClass::Get()->DisplayDialogLabel(this,tr("允许使用麦克风"),tr("请在“设置-隐私”下的“麦克风”中允许应用使用麦克风"),"",[&](QString s){
                system("Explorer.EXE ms-settings:privacy-webcam");
                return true;
            },tr("前往设置"),tr("取消"));
            return false;
        }
        else
        {
            ImageTipsWidgetWidget::DisplayDialog(this,tr("麦克风打开异常%1,msg：%2").arg(ret).arg(SRTCControl::Get()->GetCodeMsg(ret)));
            return false;
        }
    }else
    {
        SRTCControl::Get()->CloseMic();
        ui->btnMic->setImgText(tr("解除静音"),":/Images/micc.png");
    }
    myView->SetToolMic(open);
    aState = open;

    qDebug() << __func__ << "finish";
    return true;
}
bool WidRoom::OpenCloseSpeaker(bool open,QString name)
{
    qDebug()<<__func__<<open;
    if(open)
    {
        int ret = SRTCControl::Get()->OpenSpeaker(name);
        if(!ret){
            selectDefSpeaker = name.isEmpty();
        }
        else
        {
            ImageTipsWidgetWidget::DisplayDialog(this,tr("扬声器打开异常%1，msg：%2").arg(ret).arg(SRTCControl::Get()->GetCodeMsg(ret)));
            return false;
        }
    }else
    {
        SRTCControl::Get()->CloseSpeaker();
    }
    speakerState = open;
    qDebug() << __func__ << "finish";
    return true;
}


bool WidRoom::OpenCloseScreen(bool open,int hwnd,QRect rect)
{
    qDebug()<<__func__<<open<<rect;
    if(open)
    {
        int ret = SRTCControl::Get()->OpenScreen(hwnd,rect);
        if(!ret)
            ui->btnScreen->setImgText(tr("结束共享"),":/Images/bsharestop.png");
        else
        {
            ImageTipsWidgetWidget::DisplayDialog(this,tr("共享打开异常%1,msg：%2").arg(ret).arg(SRTCControl::Get()->GetCodeMsg(ret)));
            return false;
        }
    }else
    {
        SRTCControl::Get()->CloseScreen();
        ui->btnScreen->setImgText(tr("共享屏幕"),":/Images/bshare.png");
    }

    SRTCControl::Get()->UpdateRoomShareProps(open);

    sState = open;
    qDebug() << __func__ << "finish";
    return true;
}




void  WidRoom::OnUpLevel(int level){
}
void  WidRoom::OnDownLevel(QString,int){

}
void WidRoom::OnUpStat(QString updata){
    qDebug()<<__func__<<updata;
}

void WidRoom::OnDownStat(QString){

}
void WidRoom::OnSpeakers(QString speaker){
    return;
    qDebug()<<__func__<<speaker;

    QJsonDocument doc = QJsonDocument::fromJson(speaker.toLocal8Bit());
    QJsonArray array = doc.array();
    for (QJsonArray::iterator it = array.begin(); it != array.end(); ++it)
    {
        QJsonObject value = (*it).toObject();

        QString userid = value.value("userid").toString();
        if (userid == SRTCControl::Get()->sdkInfo.userid)
        {
            int db = value.value("db").toInt();
            myView->UpdateAudioDB(db);
            //底部
        }else if (memberMap.find(userid) == memberMap.end())
        {
            int db = value.value("db").toInt();
            memberMap[userid]->memview->UpdateAudioDB(db);
        }
    }
}
void WidRoom::OnDevRrecovery(int){

}
void WidRoom::OnDevChange(int,int,QString){

}
void WidRoom::OnDefDevChange(int tp,int act,QString name){
    qDebug()<<__func__<<act<<tp<<name;
    switch(tp){
        case 1:{
            if(SRTCControl::Get()->IsOpenMic() && selectDefMic){
                qDebug()<<__func__<<"re open mic";
                OpenCloseMic(true);
            }
        }
        break;
        case 2:
        {
            if(SRTCControl::Get()->IsOpenSpeaker() && selectDefSpeaker){
                qDebug()<<__func__<<"re open speaker";
                OpenCloseSpeaker(true);
            }
        }
        break;
    }

    qDebug() << __func__ << "finish";
}
void WidRoom::OnMemberUserUpdate(QString user){
    qDebug() << __func__ << user;
    QJsonObject obj = JsonUtil::GetJsonObjectFromString(user.toStdString());
    MemberDataModel data;
    data.Load(obj);
    qDebug()<<__func__<<data.uid()<<SRTCControl::Get()->sdkInfo.userid;//<<user;
    if(data.uid() != SRTCControl::Get()->sdkInfo.userid){
        bool ret = MemberUpdate(data);
        if(ret){
            ImageTipsWidgetWidget::DisplayDialog(this,tr("成员加入房间，uid:%1 , 昵称：%2").arg(data.uid()).arg(data.name()));
        }
    }
}

void WidRoom::OnMemberUserLeave(QString user)
{
    qDebug() << __func__ << user;
    QJsonObject obj = JsonUtil::GetJsonObjectFromString(user.toStdString());
    MemberDataModel data;
    data.Load(obj);
    qDebug()<<__func__<<data.uid()<<SRTCControl::Get()->sdkInfo.userid;
    MemberLeave(data.uid());
    qDebug()<<__func__<<"cur member size:"<<(memberMap.size()+1);
    ImageTipsWidgetWidget::DisplayDialog(this,tr("成员离开房间，uid:%1 , 昵称：%2").arg(data.uid()).arg(data.name()));
}

void WidRoom::OnRoomUpdate(QString room)
{
    if (needCloseRoom) {
        return;
    }
    qDebug()<<__func__<<room;
    RoomDataModel rm;
    rm.Load(JsonUtil::GetJsonObjectFromString(room.toStdString()));


    QString shareState = rm.props().shareState();
    QString ss_msg;
    if (shareState == SHARESTARE_NONE) {
        ss_msg = tr("结束了共享");
    }
    else if (shareState == SHARESTARE_DESKTOP) {
        ss_msg = tr("开启了共享");
    }
    else if (shareState == SHARESTARE_WHITEBOARD) {
        ss_msg = tr("开启了白板共享");
    }
    else {
        ss_msg = tr("未知共享状态");
    }
    if(rm.props().uid() == SRTCControl::Get()->sdkInfo.userid){
        QString name = SRTCControl::Get()->sdkInfo.userName;

        ImageTipsWidgetWidget::DisplayDialog(this,name + ss_msg);
    }
    else if(memberMap.contains(rm.props().uid()))
    {
        QString name = memberMap[rm.props().uid()]->data.name();
        ImageTipsWidgetWidget::DisplayDialog(this,name + ss_msg);
         if(memberMap[rm.props().uid()]->memview->GetTrack() == 2){
             SRTCControl::Get()->UnLoadRemoteVideoData(rm.props().uid());
             memberMap[rm.props().uid()]->memview->SetShowView(false);
             memberMap[rm.props().uid()]->memview->SetTrack(-1);
         }
    }
}


void WidRoom::on_btnChat_clicked()
{
    qDebug() << __func__ ;
    ChatControl::Get()->ShowDialog();
}


void WidRoom::on_btnScreen_clicked()
{
    qDebug() << __func__;
    OpenCloseScreen(!sState);
}

void WidRoom::on_btnMic_clicked()
{
    qDebug() << __func__;
    int v = !aState;
    if(v){
        DeviceList vdl;
        SRTCControl::Get()->EnumMicDev(vdl);
        QList<QString> ls;
        qDebug()<<vdl.GetDeviceSize();
        if(vdl.GetDeviceSize()){
            ls.append(VIEW_DEFAULT_DEV);
        }
        for(int i = 0;i<vdl.GetDeviceSize();i++){
            ls.append(vdl.GetDevice(i).GetName());
        }
        if(ls.isEmpty()){
            ls.append(tr("空"));
        }
        QString arg = ls.join(",");
        GlobalDataClass::Get()->DisplayDialogRadio(this,tr("选择麦克风设备"),arg,selectDefMicName,[&,v](QString s){
            qDebug()<<__func__<<"select mic dev :"<<s;
            selectDefMicName = s;
            if(s != tr("空")){
                if(s == VIEW_DEFAULT_DEV){
                    s = "";
                }
                OpenCloseMic(v,s);
            }
            return true;
        });
    }else{
        OpenCloseMic(v);
    }

    qDebug() << __func__ << "finish";
}


void WidRoom::on_btnSpeaker_clicked()
{
    qDebug() << __func__;
    int v = !speakerState;
    if(v){
        DeviceList vdl;
        SRTCControl::Get()->EnumSpeakerDev(vdl);
        QList<QString> ls;
        qDebug()<<vdl.GetDeviceSize();
        if(vdl.GetDeviceSize()){
            ls.append(VIEW_DEFAULT_DEV);
        }
        for(int i =0;i<vdl.GetDeviceSize();i++){
            ls.append(vdl.GetDevice(i).GetName());
        }
        if(ls.isEmpty()){
            ls.append(tr("空"));
        }
        QString arg = ls.join(",");
        GlobalDataClass::Get()->DisplayDialogRadio(this,tr("选择扬声器设备"),arg,selectDefSpeakerName,[&,v](QString s){
            qDebug()<<__func__<<"select speaker dev :"<<s;
            selectDefSpeakerName = s;
            if(s != tr("空")){
                if(s == VIEW_DEFAULT_DEV){
                    s = "";
                }
                OpenCloseSpeaker(v,s);
            }
            return true;
        });
    }else{
        OpenCloseSpeaker(v);
    }

    qDebug() << __func__ << "finish";
}

void WidRoom::on_btnCamera_clicked()
{
    qDebug() << __func__;
    int v = !vState;
    if(v){
        VideoDeviceList vdl;
        SRTCControl::Get()->EnumCameraDev(vdl);
        QList<QString> ls;
        qDebug()<<vdl.GetDeviceSize();
        for(int i =0;i<vdl.GetDeviceSize();i++){
            ls.append(vdl.GetDevice(i).GetName());
        }
        if(ls.isEmpty()){
            ls.append(tr("空"));
        }
        QString arg = ls.join(",");

        GlobalDataClass::Get()->DisplayDialogRadio(this,tr("选择摄像头设备"),arg,selectDefCameraName,[&,v](QString s){
            qDebug()<<__func__<<"select camera dev :"<<s;
            selectDefCameraName = s;
            if(s != tr("空")){
                OpenCloseCamera(v,s);
            }
            return true;
        });
    }else{
        OpenCloseCamera(v);
    }

    qDebug() << __func__ << "finish";
}

void WidRoom::on_btnExit_clicked()
{
    qDebug() << __func__;
    GlobalDataClass::Get()->DisplayDialogLabel(this,tr("确定要离开房间吗？"),"","",[&](QString s){

        qDebug() << "on_btnExit_clicked deal" ;
        CloseRoom();
        return true;
    });
}

void WidRoom::CloseRoom(bool v)
{
    qDebug()<<__func__<<"begin";
    needCloseRoom = true;
    viewindex = 0;
    GlobalDataClass::Get()->DisplayDialogClose();
    ChatControl::Get()->CloseDialog();
    if (SRTCControl::Get()->IsOpenScreen()) {
        OpenCloseScreen(false);
    }

    for(auto m :memberMap){
        ui->widViews->layout()->removeWidget(m->memview);
        delete m->memview;
    }
    qDebug()<<__func__<<"close memview";
    memberMap.clear();
    vState = false;
    aState = false;
    sState = false;
    speakerState = false;
    if(myView){
        ui->widViews->layout()->removeWidget(myView);
        delete myView;
        myView = nullptr;
    }
    qDebug()<<__func__<<"close myview 1";
    timetimer->stop();
    ChatControl::Get()->ClearMsgView();
    qDebug()<<__func__<<"close chat data";
    SRTCControl::Get()->CloseRoom();
    qDebug()<<__func__<<"close room";
    if(v){
        emit ExitRoom();
    }
    qDebug()<<__func__<<"close room finish";
}

void WidRoom::OnMemberClicked()
{
    qDebug() << __func__;
    WidDiaplayView* btn = qobject_cast<WidDiaplayView*>(sender());
    qDebug()<<__func__<<btn->GetKey();
    QString key = btn->GetKey();
    if(key == SRTCControl::Get()->sdkInfo.userid){

    }else{
        chkmember = key;
        int curTrack = btn->GetTrack();
        QList<QString> lslist;
        QMap<QString,int> maps;
        maps.insert(tr("空"),-1);
        lslist.append(tr("空"));
        QString curselect = "";
        int idlist[10] = {0};
        
        //QMap<int, int> idMap;
        for(auto s : memberMap[key]->data.streams()){
            if(s.id() == -1){
                continue;
            }
            if (s.type() < 10) {
                idlist[s.type()] = s.id() + 1;
            }
        }
        for (int i = 0; i < 10; i++) {
            QString nna = "";
            if (idlist[i]) {
                int id = idlist[i] - 1;
                switch (i) {
                case 0:
                    nna = tr("大流");
                    break;
                case 1:
                    nna = tr("小流");
                    break;
                case 2:
                    nna = tr("共享流");
                    break;
                default:
                    nna = tr("未知") + QString::number(i);
                }
                if (id == curTrack) {
                    curselect = nna;
                }
                lslist.append(nna);
                maps.insert(nna, id);
            }
        }

        QString arg;
        arg = lslist.join(",");
        QString out;
        QFont f;
        f.setBold(true);
        f.setPixelSize(24);
        QFontMetrics fontWidth(f);//得到每个字符的宽度
        QString elideNote = fontWidth.elidedText(memberMap[key]->data.name(), Qt::ElideMiddle, 130);//最大宽度150像素
        GlobalDataClass::Get()->DisplayDialogRadio(this,elideNote + tr("发布的视频流轨道"),arg,curselect,[&,key,maps,curTrack,elideNote](QString out){
            int tk = maps[out];
            qDebug()<<__func__<<elideNote<<tk<<curTrack;
            WidDiaplayView* view = memberMap[key]->memview;
            if(tk == -1){
                view->SetTrack(-1);
                view->SetShowView(false);
                SRTCControl::Get()->UnLoadRemoteVideoData(key);
            }else{
                qDebug()<<__func__<<key<<tk;
                view->SetTrack(tk);
                view->SetShowView(true);
                SRTCControl::Get()->LoadRemoteVideoData(key,tk,(void*)view->GetShowViewWidget()->winId());
            }
            return true;
        });
    }
    qDebug() << __func__ << "finish";
}

void WidRoom::OnNameClicked()
{
    qDebug() << __func__;
    QString out;
    QString arg = SRTCControl::Get()->sdkInfo.userName;
    GlobalDataClass::Get()->DisplayDialogLed(this,tr("修改昵称"),arg,tr("请输入昵称"),[&](QString out){
        
        out = out.trimmed();
        if(out.isEmpty()){
            return false;
        }
        SRTCControl::Get()->UpdateNickName(out);
        myView->SetToolName(out + tr(" (我)"));
        ChatControl::Get()->UpdateNickName(SRTCControl::Get()->sdkInfo.userid,out);
        return true;
    });

    qDebug() << __func__ << "finish";
}

void WidRoom::on_labTitle_clicked()
{
    qDebug() << __func__;
    if(!widSetData)
    {
        widSetData = new WidSetData();
        connect(widSetData,SIGNAL(NameClicked()),this,SLOT(OnNameClicked()));
    }
    SdkInfo si = SRTCControl::Get()->sdkInfo;
    widSetData->SetUIData(si.userid,si.userName,si.userPortrait);
    widSetData->exec();
}

void WidRoom::reflashAllView()
{
    qDebug() << __func__;
    for(auto m : memberMap){
        if(m->memview){
             int ret = m->memview->GetTrack();
             if(ret >= 0){
                 SRTCControl::Get()->ReflashView(m->data.uid());
             }
        }
    }
}
void WidRoom::OnWheelChange()
{
    reflashAllView(); 
}
#include "WidDeviceList.h"
void WidRoom::on_btnMicMore_clicked()
{
    WidDeviceList* w = WidDeviceList::InitAudioDevices(DEFAULTDEVICENAME, DEFAULTDEVICENAME);
    if (w) {
        w->show();
        w->setFixedWidth(250 + 40);
        QPoint p = ui->btnMicMore->mapToGlobal(QPoint(0, 0));
        p.setX(p.x() + (ui->btnMicMore->width() - (250 + 40)) / 2);
        p.setY(p.y() - w->height());
        w->move(p);
    }
}
void WidRoom::on_btnCameraMore_clicked()
{
    WidDeviceList* w = WidDeviceList::InitVideoDevices("");
    if (w) {
        w->show();
        w->setFixedWidth(250 + 40);
        QPoint p = ui->btnCameraMore->mapToGlobal(QPoint(0, 0));
        p.setX(p.x() + (ui->btnCameraMore->width() - (250 + 40)) / 2);
        p.setY(p.y() - w->height());
        w->move(p);
    }
}

void WidRoom::on_btnTest_clicked()
{

//    freopen("out.txt", "wb+", stdout);
//    SRTCControl::Get()->ReOpenSpeaker();
}

