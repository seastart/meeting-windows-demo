#include "WidRegister.h"
#include <QGraphicsDropShadowEffect>
#include <QTimer>
#include "../../NetWork/WebApiClass.h"
#pragma execution_character_set("utf-8")
WidRegister::WidRegister(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.btnRegister->setProperty("ShowType", "Normal");

	QGraphicsDropShadowEffect* shadow_effect = new QGraphicsDropShadowEffect(this);
	shadow_effect->setOffset(3, 3);              //阴影的偏移量
	shadow_effect->setColor(QColor(0x00, 0x00, 0x00, 50)); //阴影的颜色
	shadow_effect->setBlurRadius(8);             // 阴影圆角的大小
	ui.widget_2->setGraphicsEffect(shadow_effect);



    QLabel* lbl2 = new QLabel(ui.ledName);
    lbl2->setPixmap(QPixmap(":/Images/leduser.png"));
    lbl2->resize(16, 16);
    lbl2->move(16, (40 - 16) / 2 + 2);

    QLabel* lbl3 = new QLabel(ui.ledPass);
    lbl3->setPixmap(QPixmap(":/Images/ledpass.png"));
    lbl3->resize(16, 16);
    lbl3->move(16, (40 - 16) / 2 + 2);

    btn = new QPushButton(ui.ledPass);
    btn->setStyleSheet("border:0px;");
    btn->setIcon(QIcon(":/Images/ledeye2.png"));
    btn->resize(16, 16);
    QCursor cur;
    cur.setShape(Qt::CursorShape::ArrowCursor);
    btn->setCursor(cur);
    connect(btn, SIGNAL(clicked()), this, SLOT(OnPassEyeClicked()));


    QLabel* lbl4 = new QLabel(ui.ledPass2);
    lbl4->setPixmap(QPixmap(":/Images/ledpass.png"));
    lbl4->resize(16, 16);
    lbl4->move(16, (40 - 16) / 2 + 2);

    btn2 = new QPushButton(ui.ledPass2);
    btn2->setStyleSheet("border:0px;");
    btn2->setIcon(QIcon(":/Images/ledeye2.png"));
    btn2->resize(16, 16);
    QCursor cur2;
    cur2.setShape(Qt::CursorShape::ArrowCursor);
    btn2->setCursor(cur);
    connect(btn2, SIGNAL(clicked()), this, SLOT(OnPassEyeClicked2()));


    ui.ledPass->setEchoMode(QLineEdit::Password);
    ui.ledPass2->setEchoMode(QLineEdit::Password);

    codeTimer = new QTimer(this);
    connect(codeTimer,SIGNAL(timeout()),this,SLOT(OnCodeTimer()));
    codeCount = 0;
}

WidRegister::~WidRegister()
{

}


void WidRegister::initData()
{
    QTimer::singleShot(0, [&] {
        btn2->move(ui.ledPass2->width() - 32, (40 - 16) / 2 + 2);
        });
    QTimer::singleShot(0, [&] {
        btn->move(ui.ledPass->width() - 32, (40 - 16) / 2 + 2);
        });
}

void WidRegister::on_btnCode_clicked()
{
    codeCount = 60;
    codeTimer->start(1000);
    ui.btnCode->setEnabled(false);
    OnCodeTimer();
}

void WidRegister::OnCodeTimer()
{
    if (codeCount) {
        ui.btnCode->setText(tr("%1s后重发").arg(codeCount));
        codeCount--;
    }
    else {
        codeTimer->stop();
        ui.btnCode->setEnabled(true);
        ui.btnCode->setText(tr("发送验证码"));
    }
}

void WidRegister::on_btnRegister_clicked()
{

	emit RegisterFinish();
}
void WidRegister::on_btnBack_clicked()
{
	emit RegisterBack();
}
void WidRegister::OnPassEyeClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (ui.ledPass->echoMode() == QLineEdit::Password)
    {
        ui.ledPass->setEchoMode(QLineEdit::Normal);
        btn->setIcon(QIcon(":/Images/ledeye.png"));
    }
    else
    {
        ui.ledPass->setEchoMode(QLineEdit::Password);
        btn->setIcon(QIcon(":/Images/ledeye2.png"));
    }
}
void WidRegister::OnPassEyeClicked2()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (ui.ledPass2->echoMode() == QLineEdit::Password)
    {
        ui.ledPass2->setEchoMode(QLineEdit::Normal);
        btn->setIcon(QIcon(":/Images/ledeye.png"));
    }
    else
    {
        ui.ledPass2->setEchoMode(QLineEdit::Password);
        btn->setIcon(QIcon(":/Images/ledeye2.png"));
    }
}