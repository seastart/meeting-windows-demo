#pragma once

#include <QObject>
#include <QMap>
#include <QDir>
#include <QDebug>

#include <QSettings>

#include "../CJsonObject/JsonUtil.h"
#include "../View/Common/WidDialog.h"


class GlobalDataClass : public QObject
{
	Q_OBJECT

public:

	//单利
	static GlobalDataClass* Get()
	{
		if (!globalDataClass)
        {
			globalDataClass = new GlobalDataClass();
            globalDataClass->InitGlobal();
		}
		return globalDataClass;
	}
public:
	QString DataPath();
    QString PortraitPath();
    QString GetVersion();

    QString GetSettingToken(){
        return GetSetting("Token");
    }
    void SetSettingToken(QString data){
        SetSetting("Token",data);
    }
    QString GetSettingDomain(){
        return GetSetting("Domain");
    }
    void SetSettingDomain(QString data){
        SetSetting("Domain",data);
    }
    QString GetSettingName(){
        return GetSetting("Name");
    }
    void SetSettingName(QString data){
        SetSetting("Name",data);
    }
    QString GetSettingRoomId(){
        return GetSetting("RoomId");
    }
    void SetSettingRoomId(QString data){
        SetSetting("RoomId",data);
    }

    QString GetSettingOpenRoomInfo() {
        return GetSetting("OpenRoomInfo");
    }
    void SetSettingOpenRoomInfo(QString data) {
        SetSetting("OpenRoomInfo", data);
    }

    void SetSetting(QString  v,QString data){
        if(settings){
            settings->setValue(v,data);
        }
    }
    QString GetSetting(QString v){
        if(settings){
            return settings->value(v).toString();
        }
        return "";
    }
    void DisplayDialogRadio(QWidget* w,QString title,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,WidDialog::DialogColor Color = WidDialog::DialogColor::DialogColor_Blue);
    void DisplayDialogLed(QWidget* w,QString title,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,WidDialog::DialogColor Color = WidDialog::DialogColor::DialogColor_Blue);
    void DisplayDialogLabel(QWidget* w,QString title,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,QString btn1 = "",QString btn2 = "",WidDialog::DialogColor Color = WidDialog::DialogColor::DialogColor_Blue);
    void DisplayDialogClose();

private:
    void ShowDiaplayDialog(QWidget* w,QString title,QString btn1,QString btn2,WidDialog::DialogType type,QString type_arg,QString arg2,std::function<bool(QString byte)> func_ok,WidDialog::DialogColor Color);
	static GlobalDataClass* globalDataClass;
    void InitGlobal();
    QString portraitPath;
	QString dataPath;
    QSettings *settings = nullptr;
    WidDialog* _wd = nullptr;
};
