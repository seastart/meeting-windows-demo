#include "widget.h"
#include "ui_widget.h"
#include "View/Login/WidLogin.h"
#include "View/Home/WidHome.h"
#include "View/Room/WidRoom.h"
#include "View/Login/WidRegister.h"

#include "SMeetingSdk/SMeetControl.h"
#include "View/Common/ImageTipsWidgetWidget.h"
#include "View/Common/ChatControl.h"
#include "NetWork/HttpNetwork.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(SMeetControl::Get(),SIGNAL(StatusChange(int,QString)),this,SLOT(OnStatusChange(int,QString)));

    _widLogin = new WidLogin(ui->widLogin);
    connect(_widLogin, SIGNAL(Register()), this, SLOT(OnRegister()));
    connect(_widLogin, SIGNAL(LoginFinish()), this, SLOT(OnLoginFinish()));
    ui->widLogin->layout()->addWidget(_widLogin);


    _widHome = new WidHome(ui->widHome);
    connect(_widHome,SIGNAL(UnLogin()),this,SLOT(OnUnLogin()));
    connect(_widHome, SIGNAL(JoinFinish(bool,bool)), this, SLOT(OnJoinFinish(bool,bool)));
    ui->widHome->layout()->addWidget(_widHome);

    _widRoom = new WidRoom(ui->widRoom);
    connect(_widRoom,SIGNAL(ExitRoom()),this,SLOT(OnExitRoom()));
    ui->widRoom->layout()->addWidget(_widRoom);


    _widRegister = new WidRegister(ui->widRegister);
    connect(_widRegister, SIGNAL(RegisterFinish()), this, SLOT(OnRegisterFinish()));
    connect(_widRegister, SIGNAL(RegisterBack()), this, SLOT(OnUnLogin()));
    ui->widRegister->layout()->addWidget(_widRegister);


    ShowViewType(ui->widLogin);
    //setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);


    this->setFixedSize(1214,749);
    this->setWindowTitle("srtc");

}
Widget::~Widget()
{
    delete ui;
}

void Widget::hideEvent(QHideEvent *event)
{
    ChatControl::Get()->CloseDialog();
}


void Widget::showEvent(QShowEvent *event)
{
    _widRoom->reflashAllView();
}

void Widget::OnLoginFinish()
{
    qDebug()<<__func__;
    _widHome->InitData();
    ShowViewType(ui->widHome);
}
void Widget::OnJoinFinish(bool m,bool c)
{
    qDebug()<<__func__;
    _widRoom->InitData(m,c);
    ShowViewType(ui->widRoom);
}

void Widget::OnUnLogin(){
    qDebug()<<__func__;
    ShowViewType(ui->widLogin);
}
void Widget::OnExitRoom(){
    qDebug()<<__func__;
    _widHome->ReUpdate();
    ShowViewType(ui->widHome);
}
void Widget::OnRegister()
{
    qDebug() << __func__;
    _widRegister->initData();
    ShowViewType(ui->widRegister);
}

void Widget::OnRegisterFinish()
{
    qDebug() << __func__;
    _widHome->ReUpdate();
    ShowViewType(ui->widHome);
}


void Widget::ShowViewType(QWidget* w)
{
    ui->stackedWidget->setCurrentWidget(w);
}

void Widget::OnStatusChange(int code,QString message){
//    qDebug()<<__func__<<code<<message;
    QString msg = "code:"+QString::number(code) + ",message:"+message;
    ImageTipsWidgetWidget::DisplayDialog(this,msg);
    if(code == (int)SMeeting::StatusCode::SessionKickout){
        qDebug()<<__func__<<"1105 begin";
        _widRoom->CloseRoom(false);
        qDebug()<<__func__<<"1105 CloseRoom";
        SMeetControl::Get()->Logout();
        qDebug()<<__func__<<"1105 Logout";
        ShowViewType(ui->widLogin);
        qDebug()<<__func__<<"1105 view finish";
    }else if(code ==  (int)SMeeting::StatusCode::VideoCapturerError){
        _widRoom->OpenCloseCamera(false);
    }else if(code ==  (int)SMeeting::StatusCode::AudioCapturerError){
        _widRoom->OpenCloseMic(false);
    }else if(code ==  (int)SMeeting::StatusCode::AudioPlayerError){
        _widRoom->OpenCloseSpeaker(false);
    }
}
