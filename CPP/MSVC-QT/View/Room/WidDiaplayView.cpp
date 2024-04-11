#include "WidDiaplayView.h"
#include "ui_WidDiaplayView.h"
#include "../../Tools/SetImageClass.h"
#include <QDebug>
WidDiaplayView::WidDiaplayView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidDiaplayView),
    widTile(nullptr)
{
    ui->setupUi(this);
    qDebug() << __func__;
    _track = -1;
    InitTileView();
    SetToolCam(false);
    //ui->stackedWidget->setCurrentWidget(ui->page_def);
}

WidDiaplayView::~WidDiaplayView()
{
    qDebug() << __func__;
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
void WidDiaplayView::SetToolMic(bool v)
{
#ifdef DRAEMICICON
    ledMic->OpenDev(v);
#else
    ledMic->setPixmap(QPixmap(v?":/Images/minmic.png":":/Images/minmicc.png"));
#endif
    //ledMic->setText((v?"m1":"m0"));
}

void WidDiaplayView::SetToolPortrait(QString p)
{
    _portrait = p;
    SetImageClass::SetPortraitImage(ui->label_5,_key,p,":/Images/avatar.png");
}

void WidDiaplayView::SetToolCam(bool v)
{
    ledCam->setPixmap(QPixmap(v?":/Images/mincam.png":":/Images/mincamc.png"));
    //ledCam->setText((v?"c1":"c0"));

}

void WidDiaplayView::UpdateAudioDB(int db)
{
#ifdef DRAEMICICON
    if (ledMic) {
        ledMic->AddDb(db);
    }
#endif
}

void WidDiaplayView::SetShowView(bool v)
{
    if(!v){
        lblView->setUpdatesEnabled(true);
        ui->widget_back->show();
        //ui->stackedWidget->setCurrentWidget(ui->page_def);
    }else{
        lblView->setUpdatesEnabled(false);
        ui->widget_back->hide();
        //ui->stackedWidget->setCurrentWidget(ui->page_cam);
    }
}

void WidDiaplayView::mousePressEvent(QMouseEvent *event){
    QWidget::mousePressEvent(event);
    emit Clicked();
}
void WidDiaplayView::resizeEvent(QResizeEvent *event){
    TileViewMove();
    if(lblView){
        lblView->resize(event->size());
    }
    
    QWidget::resizeEvent(event);
}

void WidDiaplayView::InitTileView()
{
    qDebug() << __func__;
    widTile = new QWidget(this);
    widTile->setObjectName("widTile");
    widTile->setStyleSheet("QWidget#widTile{background:#333;border-radius:8px;}QWidget{color:#fff;}");
    QHBoxLayout *l = new QHBoxLayout(widTile);
    l->setSpacing(6);
    l->setContentsMargins(10,8,10,8);
    widTile->setLayout(l);
#ifdef DRAEMICICON
    ledMic = new MicIconView(widTile);
    //ledMic->setText("m0");
    ledMic->SetBackColor(QColor(0xff, 0xff, 0xff));
#else
    ledMic = new QLabel(widTile);
    ledMic->setText("m0");
#endif
    ledMic->setFixedSize(16,16);
    ledName = new QLabel(widTile);
    ledName->setText("");
    ledName->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ledCam = new QLabel(widTile);
    ledCam->setFixedSize(16,16);
    ledCam->setText("c0");
    l->addWidget(ledMic);
    l->addWidget(ledCam);
    l->addWidget(ledName);
    widTile->setMinimumWidth(100);
    widTile->setMaximumWidth(200);
    widTile->adjustSize();
    widTile->show();

    lblView = new QLabel(this);
    lblView->lower();
    lblView->show();
    TileViewMove();
}

void WidDiaplayView::TileViewMove()
{
    if(widTile){
        widTile->move(10,this->height() - 10 - widTile->height());
    }
}


QWidget* WidDiaplayView::GetShowViewWidget()
{
    return lblView;
}
