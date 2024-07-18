#include "WidDeviceList.h"
#include <QGraphicsDropShadowEffect>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include "SMeetingSdk/VideoDevice.h"
#pragma execution_character_set("utf-8")
WidDeviceList::WidDeviceList(QWidget *parent)
	: TanChuWid(parent)
{
	ui.setupUi(this);
    QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect(this);
    shadow_effect->setOffset(3, 3);              //阴影的偏移量
    shadow_effect->setColor(QColor(0x80, 0x80, 0x80, 0x1f)); //阴影的颜色
    shadow_effect->setBlurRadius(3);             // 阴影圆角的大小
    this->setGraphicsEffect(shadow_effect);
    this->SetType(2);
    this->SetMarginRadio(10, 8);
    this->SetTriangle(16, 10);
    this->SetMove(0.5);
}

WidDeviceList::~WidDeviceList()
{

}

WidDeviceList* WidDeviceList::InitAudioDevices(QString curMic,QString curSpk)
{
    DeviceList a_dl;
    SMeetControl::Get()->EnumMicDev(a_dl);
    DeviceList s_dl;
    SMeetControl::Get()->EnumSpeakerDev(s_dl);
   
    
    if (!a_dl.GetDeviceSize() && !s_dl.GetDeviceSize()) {
        return nullptr;
    }
    WidDeviceList* w = new WidDeviceList();

    QLabel* lab = new QLabel(w->ui.verticalWidget);
    lab->setText(tr("选择扬声器"));
    lab->setStyleSheet("color:#999;");
    w->ui.verticalWidget->layout()->addWidget(lab);

    if (s_dl.GetDeviceSize()) {
        w->AddDevice(1, DEFAULTDEVICENAME, DEFAULTDEVICENAME == curSpk);
    }
    for (int i = 0; i < s_dl.GetDeviceSize(); i++) 
    {
        auto dd = s_dl.GetDevice(i);
        w->AddDevice(1,dd.GetName(),dd.GetName() == curSpk);
    }

    QLabel* lab3 = new QLabel(w->ui.verticalWidget);
    lab3->setStyleSheet("background:#ddd");
    lab3->setFixedHeight(1);
    w->ui.verticalWidget->layout()->addWidget(lab3);

    QLabel* lab2 = new QLabel(w->ui.verticalWidget);
    lab2->setText(tr("选择麦克风"));
    lab2->setStyleSheet("color:#999;");
    w->ui.verticalWidget->layout()->addWidget(lab2);
    if (a_dl.GetDeviceSize()) {
        w->AddDevice(1, DEFAULTDEVICENAME, DEFAULTDEVICENAME == curMic);
    }
    for (int i = 0; i < a_dl.GetDeviceSize(); i++)
    {
        auto dd = a_dl.GetDevice(i);
        w->AddDevice(1, dd.GetName(), dd.GetName() == curMic);
    }

    return w;
}

WidDeviceList* WidDeviceList::InitVideoDevices(QString cur)
{
    VideoDeviceList a_dl;
    SMeetControl::Get()->EnumCameraDev(a_dl);


    if (!a_dl.GetDeviceSize()) {
        return nullptr;
    }
    WidDeviceList* w = new WidDeviceList();

    QLabel* lab = new QLabel(w->ui.verticalWidget);
    lab->setText(tr("选择摄像头"));
    lab->setStyleSheet("color:#999;");
    w->ui.verticalWidget->layout()->addWidget(lab);

    for (int i = 0; i < a_dl.GetDeviceSize(); i++)
    {
        auto dd = a_dl.GetDevice(i);
        w->AddDevice(1, dd.GetName(), dd.GetName() == cur);
    }

    return w;
}

void WidDeviceList::AddDevice(int tp, QString name, bool v) 
{

    QCheckBox* btn = new QCheckBox(ui.verticalWidget);

    btn->setChecked(v);
    
    btn->setText(name);
    connect(btn, &QPushButton::clicked, [&, tp, name]() {
        emit DeviceClicked(tp, name);
        this->close();
        });
    ui.verticalWidget->layout()->addWidget(btn);
}
