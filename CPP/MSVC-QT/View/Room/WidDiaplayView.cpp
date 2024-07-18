#include "WidDiaplayView.h"
#include "ui_WidDiaplayView.h"
#include "../../Tools/SetImageClass.h"
#include <QDebug>
#include <qcombobox.h>
#include "LoadingWidget.h"
#pragma execution_character_set("utf-8")
WidDiaplayView::WidDiaplayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidDiaplayView),
    widTile(nullptr)
{
    ui->setupUi(this);
    _track = -1;
    netIndex = -1;
    InitTileView();
    lblLoading = new LoadingWidget(this);
    lblLoading->setColor(QColor(0xdd, 0xdd, 0xdd));
    lblLoading->resize(30, 30);
    lblLoading->hide();
    lblLoading->raise();
    lblLoading->stopAnimation();
    camViewShow = false;
}

WidDiaplayView::~WidDiaplayView()
{
    SetImageClass::StopPortraitImageDownLoad(_portrait);
    delete ui;
}
void WidDiaplayView::SetToolName(QString s)
{
    qDebug()<<__func__<<"key:"<<_key<<"old:"<<ledName->text()<<",new:"<<s;
    QString nameString = s;
    QFontMetrics fontMetrics(ledName->font());

    // 如果当前字体下，字符串长度大于label宽度
    if (fontMetrics.width(nameString) > widTile->maximumWidth() - 16-16-10-10-(8*2))
    {
        nameString = QFontMetrics(ledName->font()).elidedText(nameString, Qt::ElideMiddle, widTile->maximumWidth()- 16-16-10-10-(6*2));
    }
    ledName->setText(nameString);
    widTile->adjustSize();
}
void WidDiaplayView::SetToolMic(int v)
{
#if 0
    bool newAudioState = (v >= 0);
    QString imageName = ":/Images/bcmic.png";
    if (newAudioState)
    {
        imageName = QString(":/Images/bomic%1.png").arg(v);
    }
#endif
    QString imageName = ":/Images/bcmic.png";
    if (v)
    {
        imageName = QString(":/Images/bomic0.png");
    }

    ledMic->setPixmap(QPixmap(imageName));       //当前状态
    //ledMic->setPixmap(QPixmap(v?":/image/bomic0.png":":/image/bcmic.png"));
    //ledMic->setText((v?"m1":"m0"));
}


void WidDiaplayView::SetToolNet(int v)
{
    if (netIndex != v)
    {
        netIndex = v;
        QString netImage = QString(":/Images/net%1.png").arg(netIndex);
        ledNet->setPixmap(QPixmap(netImage));
    }
}

void WidDiaplayView::SetToolPortrait(QString p)
{
    _portrait = p;
    //mm
    //SetImageClass::SetPortraitImage(ui->label_5,_key,p,":/image/avatar.png");
}

void WidDiaplayView::SetToolCam(bool v,bool load)
{
    camViewShow = v;
    camLoading = load;
    if(!v){
        ui->widget_back->show();
        if (lblLoading) {
            lblLoading->hide();
            lblLoading->stopAnimation();
        }
    }else{
        ui->widget_back->hide();
        if (lblLoading) {
            if (load) {
                lblLoading->raise();
                lblLoading->show();
                lblLoading->startAnimation();
            }
            else {
                lblLoading->hide();
                lblLoading->stopAnimation();
            }
        }
    }


}

void WidDiaplayView::ClearView()
{
    //刷去历史残留数据
    lblView->updateFull(0,0,0);
}

void WidDiaplayView::resizeEvent(QResizeEvent *event){
    TileViewMove();
    lblLoading->move((event->size().width() - lblLoading->width()) / 2, (event->size().height() - lblLoading->height()) / 2);
 	if(lblView){
        lblView->resize(event->size());
    }
    QWidget::resizeEvent(event);
}

void WidDiaplayView::InitTileView()
{
    widTile = new QWidget(this);
    widTile->setObjectName("widTile");
    widTile->setStyleSheet("QWidget#widTile{background:#333;border-radius:8px;}QWidget{color:#fff;}QLabel{font-size:13px;}");
    QHBoxLayout *l = new QHBoxLayout(widTile);
    l->setSpacing(6);
    l->setContentsMargins(10,8,10,8);
    widTile->setLayout(l);
    ledMic = new QLabel(widTile);
    ledMic->setScaledContents(true);
    ledMic->setFixedSize(16,16);
    ledName = new QLabel(widTile);
    ledName->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ledNet = new QLabel(widTile);
    ledNet->setFixedSize(16,16);
    l->addWidget(ledMic);
    l->addWidget(ledNet);
    l->addWidget(ledName);
    widTile->setMinimumWidth(100);
    widTile->setMaximumWidth(250);
    widTile->adjustSize();
    widTile->show();

    lblView = new QtRenderView(this);
    lblView->lower();
    lblView->show();
    TileViewMove();
}

void WidDiaplayView::TileViewMove()
{
    if(widTile){
        widTile->move(10, this->height() - 10 - widTile->height());
    }
}


SMeeting::IRTCView* WidDiaplayView::GetShowViewWidget()
{
    return lblView;
}

void WidDiaplayView::SetKey(QString k)
{
    _key = k;
}
