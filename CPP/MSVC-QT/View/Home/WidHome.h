#ifndef WIDHOME_H
#define WIDHOME_H

#include <QWidget>
#include <QPushButton>
#include <QResizeEvent>
namespace Ui {
class WidHome;
}

class WidHome : public QWidget
{
    Q_OBJECT

public:
    explicit WidHome(QWidget *parent = nullptr);
    ~WidHome();

    void InitData();
    void ReUpdate();
private slots:
    void on_btnJoin_clicked();
    void on_btnCreate_clicked();

    void on_btnClose2_clicked();


    void on_btnJoin2_clicked();

    void OnChatClicked();


    void OnBtnUnLoginClicked();

    void on_btnMic_clicked();
    void on_btnCamera_clicked();
    void on_widget_4_Clicked();
    void OnCreateFinish(int, QString);
    void OnEnterRoomFinish(int , QString);
signals:
    void JoinFinish(bool,bool);
    void UnLogin();
    void JoinRoomThread(int);
private:
    Ui::WidHome *ui;
    bool curMic, curCam;
};

#endif // WIDHOME_H
