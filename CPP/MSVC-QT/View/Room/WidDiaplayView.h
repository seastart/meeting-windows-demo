#ifndef WIDDIAPLAYVIEW_H
#define WIDDIAPLAYVIEW_H

#include <QWidget>
#include <QLabel>
#include <QResizeEvent>
#include <QMouseEvent>
#include "SMeetingSDK/SMeetControl.h"
#include "SMeetingSdk/QtRenderView.h"
namespace Ui {
class WidDiaplayView;
}
class QComboBox;
class LoadingWidget;
class WidDiaplayView : public QWidget
{
    Q_OBJECT

public:
    explicit WidDiaplayView(QWidget *parent = nullptr);
    ~WidDiaplayView();
    void SetToolName(QString s);
    void SetToolMic(int v);
    void SetToolNet(int v);
    void SetToolCam(bool v,bool load = false);
    void ClearView();
    void SetToolPortrait(QString p);

    SMeeting::IRTCView* GetShowViewWidget();
    void SetKey(QString);
    QString GetKey(){
        return _key;
    }
    int GetTrack(){
        return _track ;
    }
    void SetTrack( int v){
        _track = v;
    }

protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    void InitTileView();
    void TileViewMove();
private:
    Ui::WidDiaplayView *ui;
    QWidget* widTile;
    QLabel* ledMic;
    QLabel* ledName;
    QLabel* ledNet;
    QtRenderView* lblView;
    QString _key;
    QString _portrait;
    int _track;
    bool isShow = true;
    int netIndex;
    QList<int> tracks;
    QString oldtracks;
    int shareKey = -1;
    int defTrack = -1;
    bool camViewShow;
    bool camLoading;
    bool tracks_ischange = true;
    LoadingWidget* lblLoading = nullptr;
signals:
    void trackChange(int track,bool v);

};

#endif // WIDDIAPLAYVIEW_H
