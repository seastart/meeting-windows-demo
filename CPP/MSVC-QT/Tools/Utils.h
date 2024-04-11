#pragma once

#include <QString>
#include <QWidget>

#define ENUMTOSTRING(x) #x
class Utils
{
public:
    static void SaveFile(QString FilePath, const char* data);
    static QString LoadFile(QString FilePath);
	// 获取最高层的widget
	static QWidget* GetTopWidget(QWidget* self);

    static int app_cpu();
    static float GetMemoryUse();
};
