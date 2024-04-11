#ifndef CHATYOUWIDGET_H
#define CHATYOUWIDGET_H

#include <QWidget>

namespace Ui {
class ChatYouWidget;
}

#include <QMenu>
#include <Qt>
#include <QAction>
#include <QLabel>
#include "IChatBase.h"
class ChatYouWidget : public IChatBase
{
    Q_OBJECT

public:
    explicit ChatYouWidget(QWidget *parent = nullptr);
    ~ChatYouWidget();
    void SetData(QString id, QString Name, QString portrait, QString message, bool isShowHead);
    void SetNickName(QString Name);
    void SetKey(QString id) { this->id = id; }
    QString GetKey() { return this->id; }
private:
    Ui::ChatYouWidget *ui;
    QString id;
    void WrapData(const QString& text,QLabel* lbl);
    QString SpliteText(const QFontMetrics& font, const QString& text, int nLabelSize,int &index,bool& n);
    QMenu* lableMenu;
    QAction* buttonAction1;
    QAction* buttonAction2;
    QString _portrait;
};

#endif // CHATYOUWIDGET_H
