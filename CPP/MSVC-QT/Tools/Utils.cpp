#include "Utils.h"
#include <QWidget>

#include <QDir>
#include <QDebug>
#include <string>

void Utils::SaveFile(QString FileName, const char* data)
{
    if (FileName.isEmpty() == false) {
        //创建文件对象
        QFile file;
        //关联文件名字
        file.setFileName(FileName);
        //打开文件,只写方式
        bool isOK = file.open(QIODevice::WriteOnly);
        if (isOK == true) {
            file.write(data);
            file.close();
        }
    }
}
QString Utils::LoadFile(QString FilePath)
{
    QString filePath = FilePath;
    QString array;
    if (filePath.isEmpty() == false) {
        //文件对象
        QFile file(filePath);

        //打开文件
        bool isOK = file.open(QIODevice::ReadOnly);

        if (isOK == true) {
            QTextStream in(&file);
            in.setCodec("UTF-8");
            array = in.readAll();
        }
        file.close();
    }
    return array;
}

QWidget* Utils::GetTopWidget(QWidget* self)
{
	while (self->parentWidget())
	{
		self = self->parentWidget();
	}

	return self;
}
#include <windows.h>
#include <psapi.h>
#include <sysinfoapi.h>
uint64_t file_time_2_utc(const FILETIME* ftime)
{
    LARGE_INTEGER li;

    assert(ftime);
    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}
int get_processor_number()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    return (int)info.dwNumberOfProcessors;
}
int Utils::app_cpu(){
    //cpu数量
    static int processor_count_ = -1;
    //上一次的时间
    static int64_t last_time_ = 0;
    static int64_t last_system_time_ = 0;

    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    int64_t system_time;
    int64_t time;
    int64_t system_time_delta;
    int64_t time_delta;

    int cpu = 0;

    if (processor_count_ == -1)
    {
        processor_count_ = get_processor_number();
    }

    GetSystemTimeAsFileTime(&now);

    if (!GetProcessTimes(GetCurrentProcess(), &creation_time, &exit_time,
        &kernel_time, &user_time))
    {
        // We don't assert here because in some cases (such as in the Task Manager)
        // we may call this function on a process that has just exited but we have
        // not yet received the notification.
        return -1;
    }
    system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time)) / processor_count_;
    time = file_time_2_utc(&now);

    if ((last_system_time_ == 0) || (last_time_ == 0))
    {
        // First call, just set the last values.
        last_system_time_ = system_time;
        last_time_ = time;
        return -1;
    }

    system_time_delta = system_time - last_system_time_;
    time_delta = time - last_time_;

    assert(time_delta != 0);

    if (time_delta == 0)
        return -1;

    // We add time_delta / 2 so the result is rounded.
    cpu = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
    last_system_time_ = system_time;
    last_time_ = time;
    return cpu;
}
float Utils::GetMemoryUse()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    float _mem_system = 100 - ((((DWORDLONG)status.ullAvailPhys/1024.0)/((DWORDLONG)status.ullTotalPhys/1024))*100);
    int sysMemory_M = ((DWORDLONG)status.ullTotalPhys) / 1024 / 1024;
    HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
    int memoryUsage_M = pmc.WorkingSetSize / 1024 / 1024;
    return memoryUsage_M;//memoryUsage_M * 100.0 / sysMemory_M;
}

QString Utils::GetRandData(int len)
{
    QString data = "123456789ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghigklmnopqrstuvwxyz";
    QString Rand = "";
    for (int i = 0; i < len; i++)
    {
        Rand += data.at(rand() % data.size());
    }
    return Rand;
}
