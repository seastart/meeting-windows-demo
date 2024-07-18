#pragma once

#include <QWidget>
#include "ui_WidDeviceList.h"
#include "View/Common/TanChuWid.h"
#include "SMeetingSdk/VideoDevice.h"
#include "SMeetingSdk/SMeetControl.h"
class WidDeviceList : public TanChuWid
{
	Q_OBJECT

public:
	WidDeviceList(QWidget *parent = nullptr);
	~WidDeviceList();

	static WidDeviceList* InitAudioDevices(QString curMic, QString curSpk);
	static WidDeviceList* InitVideoDevices(QString curCam);
	void AddDevice(int ,QString, bool);
signals:
	void DeviceClicked(int tp,QString name);

private:
	Ui::WidDeviceListClass ui;
};
