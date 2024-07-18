#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHideEvent>
#include <QShowEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class WidLogin;
class WidHome;
class WidRoom;
class WidRegister;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void OnLoginFinish();
    void OnJoinFinish(bool,bool);
    void OnUnLogin();
    void OnExitRoom();
    void OnStatusChange(int code,QString message);
    void OnRegister();
    void OnRegisterFinish();
private:
    void ShowViewType(QWidget* w);
protected:
    virtual void hideEvent(QHideEvent *event);
    virtual void showEvent(QShowEvent *event);
private:
    Ui::Widget *ui;
    WidLogin* _widLogin;
    WidHome* _widHome;
    WidRoom* _widRoom;
    WidRegister* _widRegister;
};
#endif // WIDGET_H
