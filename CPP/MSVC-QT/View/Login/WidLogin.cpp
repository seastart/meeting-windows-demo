#include "WidLogin.h"
#include "ui_WidLogin.h"
#include <Global/GlobalDataClass.h>

#include "NetWork/HttpNetwork.h"
#include "RtcSdk/SRTCControl.h"
#include "../SRTCControl.h"
#include "../Common/ImageTipsWidgetWidget.h"
#include "../../Tools/Utils.h"
#include "../Common/WidDialog.h"
#include <QPushButton>
#include "Global/GlobalDataClass.h"
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <qtimer.h>
#include "../../NetWork/WebApiClass.h"
#pragma execution_character_set("utf-8")
WidLogin::WidLogin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidLogin)
{
    ui->setupUi(this);
    qDebug() << __func__;
    ui->btnLogin->setProperty("ShowType", "Normal");
    ui->lblversion->setText(GlobalDataClass::Get()->GetVersion());
    ui->lblversion2->setText(SRTCControl::Get()->SdkVersion());

    ui->ledName->setText(GlobalDataClass::Get()->GetSettingName());

    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(3, 3);              //阴影的偏移量
    shadow_effect->setColor(QColor(0x00, 0x00, 0x00, 50)); //阴影的颜色
    shadow_effect->setBlurRadius(8);             // 阴影圆角的大小
    ui->verticalWidget->setGraphicsEffect(shadow_effect);

    QLabel* lbl = new QLabel(ui->ledName2);
    lbl->setPixmap(QPixmap(":/Images/leduser.png"));
    lbl->resize(16, 16);
    lbl->move(16, (40 - 16) / 2 + 2);

    QLabel* lbl2 = new QLabel(ui->ledName);
    lbl2->setPixmap(QPixmap(":/Images/leduser.png"));
    lbl2->resize(16, 16);
    lbl2->move(16, (40 - 16) / 2 + 2);

    QLabel* lbl3 = new QLabel(ui->ledPass);
    lbl3->setPixmap(QPixmap(":/Images/ledpass.png"));
    lbl3->resize(16, 16);
    lbl3->move(16, (40 - 16) / 2 + 2);

    QPushButton* btn = new QPushButton(ui->ledPass);
    btn->setStyleSheet("border:0px;");
    btn->setIcon(QIcon(":/Images/ledeye2.png"));
    btn->resize(16, 16);
    QCursor cur;
    cur.setShape(Qt::CursorShape::ArrowCursor);
    btn->setCursor(cur);
    QTimer::singleShot(0, [&, btn] {
        btn->move(ui->ledPass->width() - 32, (40 - 16) / 2 + 2);
        });
    connect(btn,SIGNAL(clicked()),this,SLOT(OnPassEyeClicked()));


    ui->ledPass->setEchoMode(QLineEdit::Password);
    
    codeTimer = new QTimer(this);
    connect(codeTimer, SIGNAL(timeout()), this, SLOT(OnCodeTimer()));
    codeCount = 0;

    on_rbtnPass_clicked();
}

WidLogin::~WidLogin()
{
    qDebug() << __func__;
    delete ui;
}

void WidLogin::on_btnCode_clicked()
{
    codeCount = 60;
    codeTimer->start(1000);
    ui->btnCode->setEnabled(false);
    OnCodeTimer();
}

void WidLogin::OnCodeTimer()
{
    if (codeCount) {
        ui->btnCode->setText(tr("%1s后重发").arg(codeCount));
        codeCount--;
    }
    else {
        codeTimer->stop();
        ui->btnCode->setEnabled(true);
        ui->btnCode->setText(tr("发送验证码"));

    }
}

void WidLogin::on_btnLogin_clicked()
{
    if (!ui->chkAgreement->isChecked()) {
        ImageTipsWidgetWidget::DisplayDialog(nullptr,tr("请先勾线隐私协议。"));
        return;
    }

    HttpNetwork::Get()->SetHost(ui->ledHost->text());

    WebApiClass::ServerUser_AuthGrant(ui->ledName2->text(), [&] (QByteArray& byte){
        QJsonObject obj = QJsonDocument::fromJson(byte).object();
        int code = -1;
        JsonUtil::GetValue(obj,"code",code);
        if (!code) {
            QString data;
            JsonUtil::GetValue(obj, "msg", data);
            SRTCControl::Get()->LoginUser(ui->ledHost->text(),);
            emit LoginFinish();
        }
        else {
            QString msg;
            JsonUtil::GetValue(obj, "msg", msg);
            ImageTipsWidgetWidget::DisplayDialog(nullptr,tr("WebApiClass::ServerUser_AuthGrant fail:")+msg);
        }
        });
}


void  WidLogin::on_rbtnPass_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void  WidLogin::on_rbtnCode_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
#include "View/Room/WidDeviceList.h"
void  WidLogin::on_btnRegister_clicked()
{
    WidDeviceList* w = WidDeviceList::InitAudioDevices(DEFAULTDEVICENAME, DEFAULTDEVICENAME);
    if (w) {
        w->show();
        w->setFixedWidth(250+40);
        QPoint p = ui->btnRegister->mapToGlobal(QPoint(0, 0));
        p.setX(p.x() + (ui->btnRegister->width() - (250 + 40)) / 2);
        p.setY(p.y() - w->height());
        w->move(p);
    }
    return;
    emit Register();
}

void WidLogin::OnPassEyeClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (ui->ledPass->echoMode() == QLineEdit::Password)
    {
        ui->ledPass->setEchoMode(QLineEdit::Normal);
        btn->setIcon(QIcon(":/Images/ledeye.png"));
    }
    else
    {
        ui->ledPass->setEchoMode(QLineEdit::Password);
        btn->setIcon(QIcon(":/Images/ledeye2.png"));
    }
}