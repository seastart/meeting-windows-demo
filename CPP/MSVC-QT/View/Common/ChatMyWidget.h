#ifndef CHATMYWIDGET_H
#define CHATMYWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QMenu>
#include <Qt>
#include <QAction>
#include "IChatBase.h"
namespace Ui {
class ChatMyWidget;
}

class ChatMyWidget : public IChatBase
{
    Q_OBJECT

public:
    explicit ChatMyWidget(QWidget *parent = nullptr);
    ~ChatMyWidget();

private:
    Ui::ChatMyWidget *ui;

    void SetData(QString id, QString Name, QString portrait, QString message, bool isShowHead);
    void SetNickName(QString name);
    QString GetNickName() { return name; }
    void SetKey(QString id) { this->id = id; }
    QString GetKey() { return this->id; }
private:

    QString id;
    void WrapData(const QString& text,QLabel* lbl);
    QString SpliteText(const QFontMetrics& font, const QString& text, int nLabelSize,int& index,bool &n);
    QMenu* lableMenu;
    QAction* buttonAction1;
    QAction* buttonAction2;
    QString name;
    QString _portrait;
};

#endif // CHATMYWIDGET_H
