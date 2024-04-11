#ifndef WIDROOM_H
#define WIDROOM_H

#include <QWidget>
#include <QList>
#include <QLabel>
#include <QTimer>
#include <QWheelEvent>
#include "RtcSdk/SRTCControl.h"
#include "../../DataModel/MemberDataModel.h"

#define VIEW_DEFAULT_DEV tr("系统默认")
namespace Ui {
class WidRoom;
}

class WidSetData;
class WidDiaplayView;
class MemberInfo
{
public:
    MemberDataModel data;
    WidDiaplayView* memview;
};
class WidRoom : public QWidget
{
    Q_OBJECT

public:
    explicit WidRoom(QWidget *parent = nullptr);
    ~WidRoom();
    void InitData(bool cam = true,bool mic = true,bool speaker = true);

    void CloseRoom(bool v = true);
    void reflashAllView();
    bool OpenCloseCamera(bool open,QString name = "");
    bool OpenCloseMic(bool open,QString name = "");
    bool OpenCloseSpeaker(bool open,QString name = "");
    bool OpenCloseScreen(bool open,int hwnd = 0,QRect rect = QRect(0,0,0,0));
private slots:

    void OnUpLevel(int);
    void OnDownLevel(QString,int);
    void OnUpStat(QString);

    void OnDownStat(QString);
    void OnSpeakers(QString);
    void OnDevRrecovery(int);
    void OnDevChange(int,int,QString);
    void OnDefDevChange(int,int,QString);
    void OnMemberUserUpdate(QString);
    void OnMemberUserLeave(QString);
    void OnRoomUpdate(QString);

    void on_btnChat_clicked();

    void on_btnScreen_clicked();

    void on_btnMic_clicked();

    void on_btnSpeaker_clicked();

    void on_btnCamera_clicked();

    void on_btnExit_clicked();

    void OnMemberClicked();
    void OnNameClicked();

    void on_labTitle_clicked();

    void OnJoinRoomFinish(QString room,QString me,QString members,QString opts);
    void on_btnTest_clicked();
    void OnWheelChange();

    void on_btnMicMore_clicked();
    void on_btnCameraMore_clicked();
private:
    WidDiaplayView* CreateMemberView(QString id,QString name,QString portrait);



    void appendMsg(QString txt);
    bool MemberUpdate(MemberDataModel & mmd,bool v = true);
    void MemberLeave(QString id);
    void CompapreMemberData(QString,MemberDataModel& n,bool& ac,bool& vc,bool& nc);

signals:
    void ExitRoom();
    void JoinFinish();
private:
    Ui::WidRoom *ui;
    QMap<QString,MemberInfo*> memberMap;
    QList<QString> reloadLeaveMap;
    bool vState = false;
    bool aState = false;
    bool sState = false;
    bool speakerState = false;
    WidDiaplayView* myView = nullptr;
    QDateTime begindt;
    QTimer* timetimer;
    WidSetData* widSetData = nullptr;
    int viewindex = 0;
    QString chkmember = "";

    bool selectDefMic = false;
    bool selectDefSpeaker = false;
    QString selectDefMicName = "";
    QString selectDefSpeakerName = "";
    QString selectDefCameraName = "";

    bool needCloseRoom;
};

#endif // WIDROOM_H
