#ifndef WIDLOGIN_H
#define WIDLOGIN_H

#include <QWidget>

namespace Ui {
class WidLogin;
}

class QTimer;
class WidLogin : public QWidget
{
    Q_OBJECT

public:
    explicit WidLogin(QWidget *parent = nullptr);
    ~WidLogin();

    void initData();

private slots:
    void on_btnLogin_clicked();

    void on_rbtnPass_clicked();
    void on_rbtnCode_clicked();

    void on_btnRegister_clicked();
    void OnPassEyeClicked();

    void on_btnCode_clicked();
    void OnCodeTimer();
signals:
    void LoginFinish();
    void Register();
private:
    Ui::WidLogin *ui;
    QTimer* codeTimer;
    int codeCount;

};

#endif // WIDLOGIN_H
