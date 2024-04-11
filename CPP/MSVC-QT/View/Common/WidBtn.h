#ifndef WIDBTN_H
#define WIDBTN_H

#include <QWidget>
#include <QPushButton>
namespace Ui {
class WidBtn;
}

class WidBtn : public QPushButton
{
    Q_OBJECT

public:
    explicit WidBtn(QWidget *parent = nullptr);
    ~WidBtn();
    void SetIconSize(int);
    void setText(const QString &text);
    void setImg(QString );
    void setImgText(QString t,QString i);
private:
    Ui::WidBtn *ui;
};

#endif // WIDBTN_H
