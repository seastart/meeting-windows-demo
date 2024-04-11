#include "WidHome.h"
#include "ui_WidHome.h"
#include "../Common/ChatControl.h"
#include "../Common/WidDialog.h"
#include "../RtcSdk/SRTCControl.h"
#include "../Common/ImageTipsWidgetWidget.h"
#include "../../DataModel/RoomDataModel.h"
#include "../Common/RoundLoadingWidget.h"
#include "Global/GlobalDataClass.h"
#include "../RtcSdk/SRTCControl.h"
#include <QGraphicsDropShadowEffect>
#include "WidUserInfo.h"

#pragma execution_character_set("utf-8")
WidHome::WidHome(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidHome)
{
    ui->setupUi(this);

    qDebug() << __func__;

    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(3, 3);              //阴影的偏移量
    shadow_effect->setColor(QColor(0x00, 0x00, 0x00, 50)); //阴影的颜色
    shadow_effect->setBlurRadius(8);             // 阴影圆角的大小
    ui->widget_2->setGraphicsEffect(shadow_effect);

    QGraphicsDropShadowEffect* shadow_effect2 = new QGraphicsDropShadowEffect(this);
    shadow_effect2->setOffset(3, 3);              //阴影的偏移量
    shadow_effect2->setColor(QColor(0x00, 0x00, 0x00, 50)); //阴影的颜色
    shadow_effect2->setBlurRadius(8);             // 阴影圆角的大小
    ui->widget_3->setGraphicsEffect(shadow_effect2);

    ui->btnCreate->setProperty("ShowType", "Normal");
    ui->btnJoin->setProperty("ShowType", "NormalCancel");
    ui->btnJoin2->setProperty("ShowType", "Normal");


    QString roomInfo = GlobalDataClass::Get()->GetSettingOpenRoomInfo();
    curMic = curCam = false;
    QStringList ls = roomInfo.split(",");
    if (ls.size() == 2) {
        curMic = ls.at(0) == "1";
        curCam = ls.at(1) == "1";
    }
    ui->btnMic->SetIconSize(30);
    ui->btnCamera->SetIconSize(30);
    ui->btnMic->setImgText(curMic ? tr("关闭麦克风") : tr("打开麦克风"), curMic ? ":/Images/homic.png": ":/Images/hcmic.png");
    ui->btnCamera->setImgText(curCam ? tr("关闭摄像头") : tr("打开摄像头"), curCam ? ":/Images/hocam.png" : ":/Images/hccam.png");
    //ui->btnSpeaker->setImgText(curSpeaker ? tr("关闭扬声器") : tr("打开扬声器"), curSpeaker ? ":/Images/hospeaker.png" : ":/Images/hcspeaker.png");
    
    

    ui->widget_3->hide();
}

WidHome::~WidHome()
{
    qDebug() << __func__;
    delete ui;
}
void WidHome::InitData()
{
    qDebug() << __func__;
}

void WidHome::ReUpdate()
{
    qDebug() << __func__;
    SdkInfo& si = SRTCControl::Get()->sdkInfo;
    ui->lblUser->setText(tr("uid：") + si.userid + tr("，昵称：")+si.userName);
}

void WidHome::on_btnJoin_clicked()
{
    ui->widget_2->hide();
    ui->widget_3->show();
    ui->label_8->setText(tr("加入房间"));

}

void WidHome::on_btnCreate_clicked()
{
    ui->widget_2->hide();
    ui->widget_3->show();
    ui->label_8->setText(tr("创建房间"));

}


void WidHome::on_btnJoin2_clicked()
{
    emit JoinFinish();
}

void WidHome::on_btnClose2_clicked()
{
    ui->widget_2->show();
    ui->widget_3->hide();
}

void WidHome::on_btnMic_clicked()
{
    curMic = !curMic;
    ui->btnMic->setImgText(curMic ? tr("关闭麦克风") : tr("打开麦克风"), curMic ? ":/Images/homic.png" : ":/Images/hcmic.png");
}

void WidHome::on_btnCamera_clicked()
{
    curCam = !curCam;
    ui->btnCamera->setImgText(curCam ? tr("关闭摄像头") : tr("打开摄像头"), curCam ? ":/Images/hocam.png" : ":/Images/hccam.png");

}


void WidHome::on_widget_4_Clicked()
{
    WidUserInfo* wui = new WidUserInfo();
    QPoint p = ui->widget_4->mapToGlobal(QPoint(0, 0));
    p.setY(p.y() + ui->widget_4->height() - 18);
    p.setX(p.x()- 17);
    wui->move(p);
    connect(wui,SIGNAL(BtnUnLoginClicked()),this,SLOT(OnBtnUnLoginClicked()));
    wui->show();
}


void WidHome::OnLoginFinish(QString user,QString opt)
{
    qDebug()<<__func__<<user;
    MemberDataModel lum;
    lum.Load(QJsonDocument::fromJson(user.toUtf8()).object());
    qDebug()<<__func__<<"my self:"<<lum.uid()<<lum.name();
    ui->lblUser->setText(tr("uid：") + lum.uid() + tr("，昵称：")+lum.name());
}

void WidHome::OnChatClicked()
{
    qDebug() << __func__;
    ChatControl::Get()->ShowDialog();
}


void WidHome::OnBtnUnLoginClicked()
{
    qDebug() << __func__;
    GlobalDataClass::Get()->DisplayDialogLabel(this,tr("退出登录"),tr("您确定退出登录吗"),"",[&](QString){
            SRTCControl::Get()->Logout();
            emit UnLogin();
            return true;
    },"","",WidDialog::DialogColor_Red);
}

