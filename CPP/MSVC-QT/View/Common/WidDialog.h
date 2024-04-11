#ifndef WIDDIALOG_H
#define WIDDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QRadioButton>
#include <QList>
namespace Ui {
class WidDialog;
}

class WidDialog : public QWidget
{
    Q_OBJECT

public:
    enum DialogType {
        DialogType_Label,
        DialogType_Led,
        DialogType_Radio
    };
    enum DialogColor {
        DialogColor_Blue,
        DialogColor_Red
    };

    explicit WidDialog(QWidget *parent = nullptr);
    ~WidDialog();

    void SetType(DialogType tp);
    void SetColor(DialogColor tp);
    void SetViewButton(QString ok,QString no,QString cancel);
    void SetViewMsg(QString title,QString type_arg,QString type_arg2);
    void SetViewLed(QString title,QString type_arg,QString type_arg2);
    void SetViewRadio(QString title,QString type_arg,QString type_arg2);
    void SetFunc(std::function<bool(QString byte)> func_ok);
private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:

    QString OutputData();
private:
    Ui::WidDialog *ui;
    DialogType _tp;
    QList<QRadioButton*> rbtns;

    std::function<bool(QString byte)> func_ok = nullptr;
    std::function<bool(QString byte)> func_cancel = nullptr;
    std::function<bool(QString byte)> func_no = nullptr;
};

#endif // WIDDIALOG_H
