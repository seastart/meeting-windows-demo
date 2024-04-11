#ifndef WIDDIAPLAYVIEW_H
#define WIDDIAPLAYVIEW_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QMouseEvent>
#include "../Common/MicIconView.h"
namespace Ui {
class WidDiaplayView;
}
//#define DRAEMICICON
class WidDiaplayView : public QWidget
{
    Q_OBJECT

public:
    explicit WidDiaplayView(QWidget *parent = nullptr);
    ~WidDiaplayView();
    void SetToolName(QString s);
    void SetToolMic(bool v);
    void SetToolCam(bool v);
    void SetShowView(bool v);
    void SetToolPortrait(QString p);
    QWidget* GetShowViewWidget();
    void SetKey(QString k){
        _key = k;
    }
    QString GetKey(){
        return _key;
    }
    int GetTrack(){
        return _track ;
    }
    void SetTrack(int v){
        _track = v;
    }
    void UpdateAudioDB(int db);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
private:
    void InitTileView();
    void TileViewMove();
private:
    Ui::WidDiaplayView *ui;
    QWidget* widTile;
#ifdef DRAEMICICON
    MicIconView* ledMic;
#else // DEBUG

    QLabel* ledMic;
#endif
    QLabel* ledName;
    QLabel* ledCam;
    QLabel* lblView;
    QString _key;
    QString _portrait;
    int _track;
signals:
    void Clicked();
};

#endif // WIDDIAPLAYVIEW_H
