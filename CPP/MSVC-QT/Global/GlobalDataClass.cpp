#include "GlobalDataClass.h"
#include <QStandardPaths>
#include <QApplication>
#define SETTINGFILE "setting.ini"
GlobalDataClass* GlobalDataClass::globalDataClass = nullptr;
GlobalDataClass* GlobalDataClass::Get()
{
    if (!globalDataClass)
    {
        globalDataClass = new GlobalDataClass();
        globalDataClass->InitGlobal();
    }
    return globalDataClass;
}
#pragma execution_character_set("utf-8")
void GlobalDataClass::InitGlobal(){
    QDir dir(GlobalDataClass::Get()->PortraitPath());
    if (dir.exists())
    {
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
        QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
        foreach(QFileInfo file, fileList) { //遍历文件信息
            if (file.isFile()) { // 是文件，删除
                file.dir().remove(file.fileName());
            }
        }
    }
    else
    {
        dir.mkpath(GlobalDataClass::Get()->PortraitPath());
    }
    qDebug()<<__func__<<GlobalDataClass::Get()->PortraitPath();

    settings = new QSettings(DataPath() + SETTINGFILE,QSettings::IniFormat);
}

QString GlobalDataClass::DataPath()
{
	if (dataPath.isEmpty())
	{
		QString documentPath = QStandardPaths::writableLocation(QStandardPaths::StandardLocation::DocumentsLocation);
		dataPath = documentPath + "/" + QApplication::applicationDisplayName() + "/";
	}
	return dataPath;
}

QString GlobalDataClass::PortraitPath()
{
    if(portraitPath.isEmpty()){
        portraitPath = DataPath() + "Portrait/";
    }

    return portraitPath;
}
void GetBuildData(int* y, int* m, int* d)
{
    const char* pMonth[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
    const char Date[12] = __DATE__;//取编译时间
    uint8_t i;
    for (i = 0; i < 12; i++)if (memcmp(Date, pMonth[i], 3) == 0)*m = i + 1, i = 12;
    *y = atoi(Date + 7); //Date[9]为２位年份，Date[7]为完整年份
    *d = atoi(Date + 4);
}

QString GlobalDataClass::GetVersion()
{
    QString s = "";
    int y, m, d;
    GetBuildData(&y,&m,&d);
    s += QString::number(y);
    if (m < 10) {
        s += "0";
    }
    s += QString::number(m);
    if (d < 10) {
        s += "0";

    }
    s += QString::number(d);
    QStringList lstime = QString(__TIME__).split(":");
    s += lstime[0]+lstime[1];
    return s;
}
#include "../Tools/Utils.h"
void GlobalDataClass::ShowDiaplayDialog(QWidget* w,QString title,QString btn1,QString btn2,WidDialog::DialogType type,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,WidDialog::DialogColor Color)
{
    qDebug() << __func__ << title;
    if(!_wd){
        _wd =new WidDialog();
//        if(w){
//            _wd->setParent(Utils::GetTopWidget(w));
//        }
        _wd->setWindowFlag(Qt::Window);
        _wd->setWindowFlag(Qt::WindowStaysOnTopHint);
    }

    _wd->SetType(type);
    _wd->SetColor(Color);
    if(btn1.isEmpty()){
        btn1 = tr("确定");
    }
    if(btn2.isEmpty()){
        btn2 = tr("取消");
    }
    _wd->SetViewButton(btn1,"",btn2);
    switch(type){
        case WidDialog::DialogType::DialogType_Label:
            _wd->SetViewMsg(title, type_arg,arg2);
            break;
        case WidDialog::DialogType::DialogType_Led:
            _wd->SetViewLed(title, type_arg,arg2);
            break;
        case WidDialog::DialogType::DialogType_Radio:
            _wd->SetViewRadio(title, type_arg,arg2);
            break;
    }
    _wd->setMinimumWidth(460);
    _wd->setMaximumWidth(210);
    _wd->adjustSize();
    _wd->setFixedSize(_wd->size());
    _wd->SetFunc(func_ok);
    if (w != nullptr)
    {
        w = Utils::GetTopWidget(w);

        QSize size = w->size();

        QPoint point = w->frameGeometry().topLeft();
        float x_pos = (size.width() - _wd->width()) / 2.f;
        float y_pos = (size.height() - _wd->height()) / 2.f;
        _wd->move(x_pos + point.x(), y_pos + point.y());
    }
    _wd->show();
}
void GlobalDataClass::DisplayDialogRadio(QWidget* w,QString title,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,WidDialog::DialogColor Color)
{
    ShowDiaplayDialog(w,title,"","",WidDialog::DialogType::DialogType_Radio,type_arg,arg2,func_ok,Color);
}
void GlobalDataClass::DisplayDialogLed(QWidget* w,QString title,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,WidDialog::DialogColor Color)
{
    ShowDiaplayDialog(w,title,"","",WidDialog::DialogType::DialogType_Led,type_arg,arg2,func_ok,Color);

}
void GlobalDataClass::DisplayDialogLabel(QWidget* w,QString title,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,QString btn1,QString btn2,WidDialog::DialogColor Color)
{
    ShowDiaplayDialog(w,title,btn1,btn2,WidDialog::DialogType::DialogType_Label,type_arg,arg2,func_ok,Color);
}
void GlobalDataClass::DisplayDialogClose()
{
    if(_wd){
        _wd->hide();
    }
}
