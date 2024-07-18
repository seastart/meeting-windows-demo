#ifndef QTRENDERVIEW_H
#define QTRENDERVIEW_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>
#include <SMeeting.h>
#include <mutex>

class QtRenderView : public QWidget,public SMeeting::IRTCView
{
    Q_OBJECT

public:
    QtRenderView(QWidget *parent = nullptr);
    ~QtRenderView();

    void* getHwnd() override {return 0;};
    SMeeting::ViewEnumType ViewType() override {return SMeeting::ViewEnumType::CallBack;}
    //fourcc:0 yuv,1 nv12,2 nv21,3 yuyv,4 bg32
    void updatePlanes(const unsigned char* buf, int w, int h, int fourcc, int label) override;
    void updateFull(int r,int g,int b) override;

    void set_cap_view(bool v){
        cap_view = v;
    }
    //int mirror = 0;//0 不镜像，1上下，2左右
    //int rotate = 0;//0，0度，1，90度，2，180，3，270度
    void set_morror(int v){
        custom_matrix = true;
        _morror = v;
    }
    void set_rotate(int v){
        custom_matrix = true;
        _rotate = v;
    }
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    void AdjustPicture(int w, int h, int cx, int cy, int* iLeft, int* iTop, int* iWidth, int* iHeight);
signals:
    void onNeedUpdate();
private:
    QImage* _img = nullptr;
    QImage _img3;

    bool custom_matrix = false;
    int _rotate = 0;
    int _morror = 0;
    int _w;
    int _h;
    int _label = 0;
    int Flash_Rgb[4];
    int _need_flash;
    int _stream_id;
    int _track;
    long long dt;
    int _fps;
    std::mutex _mutex;
    int cap_view = false;
};


#endif // QTRENDERVIEW_H
