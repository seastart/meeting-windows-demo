#include "widget.h"

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>
#include "Tools/Utils.h"
#include <QFile>

#include <QHBoxLayout>
#include <QLabel>
#include <View/Room/WidDiaplayView.h>
#include <QTime>
#include <QDebug>
#include "log4qt/propertyconfigurator.h"
#include "log4qt/logmanager.h"
#include <windows.h>
#include <Dbghelp.h>
#include <QSslSocket>
#include "Global/GlobalDataClass.h"
#pragma execution_character_set("utf-8")

LONG WINAPI OnUnhandleException(_In_ struct _EXCEPTION_POINTERS* ExceptionInfo)
{
    LONG rc = EXCEPTION_CONTINUE_SEARCH;

#if 1
    HMODULE hDll = NULL;

    QString dumpFile = GlobalDataClass::Get()->DataPath() + "\\";
    dumpFile += "crash.dmp";
    WCHAR wsz[256];
    memset(wsz, 0, sizeof(wsz));
    MultiByteToWideChar(CP_ACP, 0, dumpFile.toStdString().c_str(), strlen(dumpFile.toStdString().c_str()) + 1, wsz,
        sizeof(wsz) / sizeof(wsz[0]));


    HANDLE hDumpFile = CreateFile(wsz, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
        CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hDumpFile != INVALID_HANDLE_VALUE)
    {
        MINIDUMP_EXCEPTION_INFORMATION ExInfo;

        ExInfo.ThreadId = GetCurrentThreadId();
        ExInfo.ExceptionPointers = ExceptionInfo;
        ExInfo.ClientPointers = TRUE;

        // Write the information into the dump
        if (MiniDumpWriteDump(
            GetCurrentProcess(), // Handle of process
            GetCurrentProcessId(), // Process Id
            hDumpFile,    // Handle of dump file
            MiniDumpNormal,   // Dump Level: Mini
            &ExInfo,    // Exception information
            NULL,     // User stream parameter
            NULL))     // Callback Parameter
        {
            rc = EXCEPTION_CONTINUE_SEARCH;
        }
        else
        {
            //_tprintf(_T("MiniDumpWriteDump failed w/err 0x%08lx\n"), GetLastError());
        }

        CloseHandle(hDumpFile);
    }
    else
    {
        //_tprintf(_T("CreateFile failed w/err 0x%08lx\n"), GetLastError());
    }
#endif
    return rc;
}

int main(int argc, char *argv[])
{


    QApplication a(argc, argv);

    srand(time(0));
#if true
    if (QFile::exists(QCoreApplication::applicationDirPath() + "/log4qt.conf"))
    {
        qDebug() << "init log";
        Log4Qt::LogManager::setHandleQtMessages(true);
        Log4Qt::PropertyConfigurator::configure(QCoreApplication::applicationDirPath() + "/log4qt.conf");
        qDebug() << ("//////////////");
        qDebug() << QObject::tr("软件启动");
        qDebug() << ("//////////////");
    }
#endif

    qDebug() << __func__;
    qDebug() << "QSslSocket=" << QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QObject::tr("OpenSSL支持情况:") << QSslSocket::supportsSsl();
    QString qss = Utils::LoadFile(":/Qss/base.qss");
    qApp->setStyleSheet(qss);

    QFontDatabase database;
    QStringList fonts = database.families();
    if (fonts.contains(QObject::tr("微软雅黑")))
    {
        qDebug() << "find Microsoft YaHei";
        QFont font(QObject::tr("微软雅黑"));
        QApplication::setFont(font);
    }


    Widget w;
    w.show();

    SetUnhandledExceptionFilter(OnUnhandleException);
    return a.exec();
}
