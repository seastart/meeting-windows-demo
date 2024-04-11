#ifndef CHATCONTROL_H
#define CHATCONTROL_H

#include <QObject>
#include <QTimer>
#include <QList>

class IChatBase;
class FlowLayout;
class ChatYouSpeakWidget;
class ChatMySpeakWidget;
class ChatWidget;
class ChatControl : public QObject
{
    Q_OBJECT
public:
    static ChatControl* Get()
        {
            if (!_cc)
            {
                _cc = new ChatControl();
            }
            return _cc;
        }
public:
    ChatControl(QObject *parent = nullptr);
    void ShowDialog(bool show = true);
    void AddMsgView(QString sid,QString sname,QString portrait,QString msg);
    void UpdateNickName(QString tar,QString nname);
    void ClearMsgView();
    void CloseDialog();

private slots:
    void OnLongShowDateTimerOut();
    void OnSendMsg(QString tar,QString msg);
    void OnRecvMsg(QString,QString,QString);
private:
    void AddTimeDate();
private:
    static ChatControl* _cc;
    ChatWidget* _cw;
    FlowLayout* _layout;
    QTimer* _longTimer;
    bool needAddDate;
    QList<IChatBase*> chats;
};

#endif // CHATCONTROL_H
