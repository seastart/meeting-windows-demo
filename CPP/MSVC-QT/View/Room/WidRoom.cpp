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


    connect(SMeetControl::Get(), SIGNAL(UserEnter(QString, QString)), this, SLOT(OnUserEnter(QString, QString)));
    connect(SMeetControl::Get(), SIGNAL(UserExit(QString, QString)), this, SLOT(OnMemberUserLeave(QString, QString)));
    connect(SMeetControl::Get(), SIGNAL(smtCameraUpdate(int,int, QString)),this,SLOT(OnCameraUpdate(int, int, QString)));
    connect(SMeetControl::Get(), SIGNAL(smtMicUpdate(int, int, QString)), this, SLOT(OnMicUpdate(int, int, QString)));
    connect(SMeetControl::Get(), SIGNAL(smtNickNameUpdate(int, QString)), this, SLOT(OnNickNameUpdate(int, QString)));
    connect(SMeetControl::Get(), SIGNAL(smtShareUpdate(int, int, QString)), this, SLOT(OnShareUpdate(int,int,QString)));
    connect(SMeetControl::Get(), SIGNAL(smtExitRoomFinish(int,QString)), this, SLOT(OnExitRoomFinish(int, QString)));
    timetimer = new QTimer(this);
    connect(timetimer,&QTimer::timeout,[&]{
        int mt = begindt.msecsTo(QDateTime::currentDateTime());
        ui->lblTime->setText(QTime::fromMSecsSinceStartOfDay(mt).toString());
        ui->lblcpu->setText(QString("%1%").arg(Utils::app_cpu()));
        ui->lblmem->setText(QString("%1M").arg(Utils::GetMemoryUse()));
    });
    ChatControl::Get();



    myView = nullptr;
    viewindex = 0;
    init_view_finish = false;
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


void WidRoom::InitData(bool cam,bool mic,bool speaker)
{
    viewindex = 0;
    qDebug() << __func__<<cam<<mic<<speaker;
    QTimer::singleShot(0,[&,cam,mic,speaker]{
        qDebug() <<"InitData" << __func__;
        ImageTipsWidgetWidget::DisplayDialog(this,tr("加入房间成功"));
        if(!myView){
            ui->btnChat->setImgText(tr("聊天"),":/Images/bchat.png");
            ui->btnCamera->setImgText(tr("开启视频"),":/Images/camc.png");
            ui->btnMic->setImgText(tr("解除静音"),":/Images/micc.png");
            ui->btnScreen->setImgText(tr("共享屏幕"),":/Images/bshare.png");
            ui->btnMember->setImgText(tr("成员(1)"),"");

            QString roomid = SMeetControl::Get()->sdkInfo.roomid;
            roomid = QFontMetrics(ui->labTitle->font()).elidedText(roomid, Qt::ElideRight, 500);

            ui->labTitle->setText(roomid);
            myView = CreateMemberView(SMeetControl::Get()->sdkInfo.userid,SMeetControl::Get()->sdkInfo.userName,SMeetControl::Get()->sdkInfo.userPortrait);
            myView->SetToolName(SMeetControl::Get()->sdkInfo.userName+tr(" (我)"));
            SMeetControl::Get()->SetCameraView(myView->GetShowViewWidget());
            begindt = QDateTime::currentDateTime();
            timetimer->start(1000);
            qDebug()<<"InitData"<<"OpenCloseCamera"<<cam;
            if(cam)
                OpenCloseCamera(cam);
            qDebug()<<"InitData"<<"OpenCloseMic"<<mic;
            if(mic)
                OpenCloseMic(mic);
            qDebug()<<"InitData"<<"OpenCloseSpeaker"<<speaker;
            if(speaker)
                OpenCloseSpeaker(speaker);
        }

        init_view_finish = true;
        qDebug() << "InitData" << __func__ << 1;
        for (auto& m : memberMap) {
            if (!m->memview) {
                MemberDataModel& mmd = m->data;
                WidDiaplayView* l = CreateMemberView(mmd.uid(), mmd.name(), mmd.avatar());
                l->SetToolMic(mmd.props().audioState());
                l->SetToolCam(mmd.props().videoState());
                m->memview = l;
            }
        }

        qDebug() << "InitData" << __func__ << 2;

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

bool WidRoom::MemberUpdate(MemberDataModel & mmd)
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
                    memberMap[mmd.uid()]->memview->SetToolCam(mmd.props().videoState());
                    int curTrack = memberMap[mmd.uid()]->memview->GetTrack();
                    if(curTrack != -1){
                        SMeetControl::Get()->UnLoadRemoteVideoData(mmd.uid());
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
        qDebug()<<__func__<<"append member:"+mmd.uid()+",name:"+mmd.name()+"view:"+QString::number(init_view_finish);
        MemberInfo* mem = new MemberInfo();
        mem->data = mmd;
        if(init_view_finish){
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
    v->SetToolNet(4);
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
    qDebug() << __func__ << open << name;
    if(!name.isEmpty()){
        SMeetControl::Get()->SwitchCamera(name);
    }
    if(open){
        //OnCameraUpdate 成功或失败
        SMeeting::StatusCode sc = SMeetControl::Get()->OpenCamera();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 打开摄像头异常：%1").arg((int)sc));
        }
    }else{
        SMeeting::StatusCode sc = SMeetControl::Get()->CloseCamera();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 关闭摄像头异常：%1").arg((int)sc));
        }
    }
    qDebug() << __func__ << "finish";
    return true;
}

bool WidRoom::OpenCloseMic(bool open,QString name)
{
    qDebug() << __func__ << open << name;
    if (!name.isEmpty()) {
        SMeetControl::Get()->SwitchMic(name);
    }
    qDebug() << __func__;
    if (open) {
        //OnMicUpdate 成功或失败
        SMeeting::StatusCode sc = SMeetControl::Get()->OpenMic();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 打开麦克风异常：%1").arg((int)sc));
        }
    }
    else {
        SMeeting::StatusCode sc = SMeetControl::Get()->CloseMic();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 关闭麦克风异常：%1").arg((int)sc));
        }
    }

    qDebug() << __func__ << "finish";
    return true;
}
bool WidRoom::OpenCloseSpeaker(bool open,QString name)
{
    qDebug() << __func__ << open << name;
    if (!name.isEmpty()) {
        SMeetControl::Get()->SwitchSpeaker(name);
    }
    SMeeting::StatusCode sc;
    if (open) {
        sc = SMeetControl::Get()->OpenSpeaker();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 打开扬声器异常：%1").arg((int)sc));
        }
    }
    else {
        sc = SMeetControl::Get()->CloseSpeaker();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 关闭扬声器异常：%1").arg((int)sc));
        }
    }
    qDebug() << __func__ << "finish" << (int)sc;
    return true;
}


bool WidRoom::OpenCloseScreen(bool open,int hwnd,QRect rect)
{
    qDebug() << __func__ << open << hwnd<<rect;
    SMeeting::StatusCode sc;
    if (open) {
        sc = SMeetControl::Get()->OpenScreen(hwnd,rect);
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 打开共享异常：%1").arg((int)sc));
        }
    }
    else {
        sc = SMeetControl::Get()->CloseScreen();
        if (sc != SMeeting::StatusCode::OK) {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 关闭共享异常：%1").arg((int)sc));
        }
    }
    qDebug() << __func__ << "finish" << (int)sc;
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
}
void WidRoom::OnDevRrecovery(int){

}
void WidRoom::OnDevChange(int,int,QString){

}
void WidRoom::OnDefDevChange(int tp,int act,QString name){
    qDebug()<<__func__<<act<<tp<<name;
    switch(tp){
        case 1:{
            if(SMeetControl::Get()->IsOpenMic() && selectDefMic){
                qDebug()<<__func__<<"re open mic";
                OpenCloseMic(true);
            }
        }
        break;
        case 2:
        {
            if(SMeetControl::Get()->IsOpenSpeaker() && selectDefSpeaker){
                qDebug()<<__func__<<"re open speaker";
                OpenCloseSpeaker(true);
            }
        }
        break;
    }

    qDebug() << __func__ << "finish";
}
void WidRoom::OnUserEnter(QString roomid,QString user){
    return;


    qDebug() << __func__ << user;
    QJsonObject obj = JsonUtil::GetJsonObjectFromString(user.toStdString());
    MemberDataModel data;
    data.Load(obj);
    qDebug()<<__func__<<data.uid()<<SMeetControl::Get()->sdkInfo.userid;//<<user;
    if(data.uid() != SMeetControl::Get()->sdkInfo.userid){
        bool ret = MemberUpdate(data);
        if(ret){
            ImageTipsWidgetWidget::DisplayDialog(this,tr("成员加入房间，uid:%1 , 昵称：%2").arg(data.uid()).arg(data.name()));
        }
    }
}

void WidRoom::OnMemberUserLeave(QString roomid, QString user)
{
    qDebug() << __func__ << user;
    QJsonObject obj = JsonUtil::GetJsonObjectFromString(user.toStdString());
    MemberDataModel data;
    data.Load(obj);
    qDebug()<<__func__<<data.uid()<<SMeetControl::Get()->sdkInfo.userid;
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
    if(rm.props().uid() == SMeetControl::Get()->sdkInfo.userid){
        QString name = SMeetControl::Get()->sdkInfo.userName;

        ImageTipsWidgetWidget::DisplayDialog(this,name + ss_msg);
    }
    else if(memberMap.contains(rm.props().uid()))
    {
        QString name = memberMap[rm.props().uid()]->data.name();
        ImageTipsWidgetWidget::DisplayDialog(this,name + ss_msg);
         if(memberMap[rm.props().uid()]->memview->GetTrack() == 2){
             SMeetControl::Get()->UnLoadRemoteVideoData(rm.props().uid());
             memberMap[rm.props().uid()]->memview->SetToolCam(false);
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
    OpenCloseMic(v);
    qDebug() << __func__ << "finish";
}

void WidRoom::on_btnCamera_clicked()
{
    qDebug() << __func__;
    int v = !vState;
    OpenCloseCamera(v);
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
    if (SMeetControl::Get()->IsOpenScreen()) {
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
    SMeetControl::Get()->CloseRoom();
    qDebug()<<__func__<<"close room";
    qDebug()<<__func__<<"close room finish";
}

void WidRoom::OnMemberClicked()
{
    qDebug() << __func__;
    WidDiaplayView* btn = qobject_cast<WidDiaplayView*>(sender());
    qDebug()<<__func__<<btn->GetKey();
    QString key = btn->GetKey();
    if(key == SMeetControl::Get()->sdkInfo.userid){

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
                view->SetToolCam(false);
                SMeetControl::Get()->UnLoadRemoteVideoData(key);
            }else{
                qDebug()<<__func__<<key<<tk;
                view->SetTrack(tk);
                view->SetToolCam(true);
                SMeetControl::Get()->LoadRemoteVideoData(key,tk,view->GetShowViewWidget());
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
    QString arg = SMeetControl::Get()->sdkInfo.userName;
    GlobalDataClass::Get()->DisplayDialogLed(this,tr("修改昵称"),arg,tr("请输入昵称"),[&](QString out){
        
        out = out.trimmed();
        if(out.isEmpty()){
            return false;
        }
        SMeetControl::Get()->UpdateNickName(out);
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
    SdkInfo si = SMeetControl::Get()->sdkInfo;
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
                 SMeetControl::Get()->ReflashView(m->data.uid());
             }
        }
    }
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
void WidRoom::on_pushButton_clicked()
{
    //((GdiRenderView*)myView->GetShowViewWidget())->updateFull(255,0,0);

}


void WidRoom::OnCameraUpdate(int open,int code,QString data)
{
    qDebug() << __func__ << open << code << data;
    if (!code) {
        vState = open;
        myView->SetToolCam(open);
        if (open) {
            ui->btnCamera->setImgText(tr("关闭视频"), ":/Images/cam.png");
        }
        else {
            ui->btnCamera->setImgText(tr("开启视频"), ":/Images/camc.png");
        }
    }
    else {
        ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK %1摄像头异常2：%2,%3")
            .arg(open? tr("打开"): tr("关闭")).arg((int)code).arg(data));
    }
}

void WidRoom::OnMicUpdate(int open, int code , QString data)
{
    qDebug() << __func__ << open << code << data;
    if (!code) {
        aState = open;
        myView->SetToolMic(open);
        if (open) {
            ui->btnMic->setImgText(tr("关闭麦克风"), ":/Images/mic.png");
        }
        else {
            ui->btnMic->setImgText(tr("麦克风"), ":/Images/micc.png");
        }
    }
    else {
        ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK %1麦克风异常2：%2,%3")
            .arg(open ? tr("打开") : tr("关闭")).arg((int)code).arg(data));
    }
}

void WidRoom::OnNickNameUpdate(int code, QString data)
{
    if (!code) {
        myView->SetToolName(data + tr(" (我)"));
        ChatControl::Get()->UpdateNickName(SMeetControl::Get()->sdkInfo.userid, data);
    }
    else {
        ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 修改昵称异常：%1,%2")
            .arg((int)code).arg(data));
    }
}

void WidRoom::OnShareUpdate(int tp, int code, QString data)
{
    if (!code) {
        if (tp == 1)
        {
            sState = true;
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("开始共享"));
            ui->btnScreen->setImgText(tr("解除共享"), ":/Images/bsharestop.png");
        }
        else if(tp == 0){
            sState = false;
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("结束共享"));
            ui->btnScreen->setImgText(tr("共享屏幕"), ":/Images/bshare.png");
        }
        else {
            ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("not deal code:%1").arg(__func__));
        }
    }
    else {
        ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 退出会议异常：%1,%2")
            .arg((int)code).arg(data));
    }
}

void WidRoom::OnExitRoomFinish(int code, QString data)
{
    if (!code) {
        init_view_finish = false;
        emit ExitRoom();
    }
    else {
        ImageTipsWidgetWidget::DisplayDialog(nullptr, tr("SDK 退出会议异常：%1,%2")
                    .arg((int)code).arg(data));
    }
}