#ifndef WIDSETDATA_H
#define WIDSETDATA_H

#include <QDialog>

namespace Ui {
class WidSetData;
}

class WidSetData : public QDialog
{
    Q_OBJECT

public:
    explicit WidSetData(QWidget *parent = nullptr);
    ~WidSetData();

    void SetUIData(QString userid,QString name,QString Portrait);
signals:
    void NameClicked();
private slots:

    void on_pushButton_3_clicked();

private:
    Ui::WidSetData *ui;
    QString _portrait;
};

#endif // WIDSETDATA_H
