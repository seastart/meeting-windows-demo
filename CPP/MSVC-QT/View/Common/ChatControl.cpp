#include "ChatControl.h"
#include "ChatWidget.h"
#include "ChatMyWidget.h"
#include "ChatYouWidget.h"
#include "../Tools/FlowLayout.h"
#include <QDateTime>
#include "../../RtcSdk/SRTCControl.h"
#include "ImageTipsWidgetWidget.h"
ChatControl* ChatControl::_cc = nullptr;
#pragma execution_character_set("utf-8")
ChatControl::ChatControl(QObject *parent)
    : QObject{parent}
{
    _cw = nullptr;
    _layout = nullptr;
    _longTimer = nullptr;
    needAddDate = true;
    connect(SRTCControl::Get(),SIGNAL(RecvMsg(QString,QString,QString)),this,SLOT(OnRecvMsg(QString,QString,QString)));

}

void ChatControl::OnRecvMsg(QString action,QString msg,QString tar)
{
    qDebug()<<__func__<<action<<msg<<tar;
    QJsonObject obj = JsonUtil::GetJsonObjectFromString(tar.toStdString());
    QString tar_name,tar_portrait,tra_userid;
    JsonUtil::GetValue(obj,"uid",tra_userid);
    JsonUtil::GetValue(obj,"name",tar_name);
    JsonUtil::GetValue(obj,"avatar",tar_portrait);
//    ui->txtmsg->append("action:" + act + ",msg=" + msg + ",tar=" + tar_name);
    if(!_cw){
        ShowDialog(false);
    }
    AddMsgView(tra_userid,tar_name,tar_portrait,msg);
}

void ChatControl::ShowDialog(bool v)
{
    qDebug() << __func__;
    if(!_cw){
        _cw = new ChatWidget();
        connect(_cw,SIGNAL(SendMsg(QString,QString)),this,SLOT(OnSendMsg(QString,QString)));
        _layout = new FlowLayout(_cw->GetViewWidget(), 0, 0, 5);
        _cw->SetViewLayout(_layout);
    }
    if(v){
        _cw->setWindowState(Qt::WindowNoState);
        _cw->activateWindow();
        _cw->show();
    }
}

void ChatControl::OnLongShowDateTimerOut()
{
    needAddDate = true;
}
void ChatControl::OnSendMsg(QString tar,QString msg)
{
    qDebug() << __func__;
    int ret;
    if(tar.isEmpty()){
        ret = SRTCControl::Get()->SendRoomMsg("default",msg);
    }else{
        ret = SRTCControl::Get()->SendMsg(tar,"default",msg);
    }
    qDebug()<<__func__<<ret;
    if(ret){
        ImageTipsWidgetWidget::DisplayDialog(_cw,tr("发送异常%1,msg：%2").arg(ret).arg(SRTCControl::Get()->GetCodeMsg(ret)));
    }
//    if(ret){
//        AddMsgView(_myId,_myName,_myPortrait,msg);
//    }
}
void ChatControl::AddMsgView(QString sid,QString sname,QString portrait,QString msg)
{
    qDebug() << __func__;
    if (needAddDate)
    {
        AddTimeDate();
    }
    IChatBase* ichat = nullptr;
    if(sid == SRTCControl::Get()->sdkInfo.userid){
        ichat = new ChatMyWidget(_cw->GetViewWidget());
    }else{
        ichat = new ChatYouWidget(_cw->GetViewWidget());
    }
    ichat->SetKey(sid);
    ichat->SetNickName(sname);
    ichat->SetData(sid,sname,portrait,msg,true);
    ichat->setFixedWidth(_cw->size().width());
    _layout->addWidget(ichat);
    chats.append(ichat);
    if(!_longTimer){
        _longTimer = new QTimer(this);
        connect(_longTimer,SIGNAL(timeout()),this,SLOT(OnLongShowDateTimerOut()));
    }
    if (_longTimer->isActive())
    {
        _longTimer->stop();
    }
    _longTimer->start(1000*60*2);
}

void ChatControl::AddTimeDate()
{
    qDebug() << __func__;
    QWidget* wid = new QWidget(_cw);
    QHBoxLayout* layout = new QHBoxLayout(wid);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);
    wid->setLayout(layout);
    QLabel* label = new QLabel(wid);
    label->setSizePolicy(QSizePolicy::Policy::Maximum, QSizePolicy::Policy::Preferred);
    label->setStyleSheet("color:#999;border:0px;");
    label->setText(QDateTime::currentDateTime().toString(" HH:mm "));
    wid->setFixedWidth(_cw->size().width());
    layout->addWidget(label);
    _layout->addWidget(wid);
    needAddDate = false;
}

void ChatControl::UpdateNickName(QString tar,QString nname)
{

    qDebug() << __func__;
    for(auto c : chats){
        if(c->GetKey() == tar){
            c->SetNickName(nname);
        }
    }
}

void ChatControl::ClearMsgView()
{
    qDebug() << __func__;
    chats.clear();
    if (_cw && _layout)
    {
        QList<QLayoutItem*>& layoutList = _layout->getItemList();
        int lsize = layoutList.size();
        for (int i = 0; i < lsize; i++)
        {
            QLayoutItem* layoutItem = layoutList[0];
            QWidget* wid = (QWidget*)layoutItem->widget();
            _layout->removeWidget(wid);
            delete wid;
        }
    }
}


void ChatControl::CloseDialog()
{
    qDebug() << __func__;
    if (_cw)
    {
        _cw->close();
    }
}
