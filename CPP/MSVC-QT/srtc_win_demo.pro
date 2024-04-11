QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#QMAKE_CXXFLAGS_RELEASE -= -O2
RC_ICONS = favicon.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Global/GlobalDataClass.cpp \
    NetWork/HttpNetwork.cpp \
    RtcSdk/SRTCControl.cpp \
    RtcSdk/VideoDevice.cpp \
    Tools/FlowLayout.cpp \
    Tools/MyScrollArea.cpp \
    Tools/SetImageClass.cpp \
    Tools/Utils.cpp \
    Tools/log4qt/appenderskeleton.cpp \
    Tools/log4qt/basicconfigurator.cpp \
    Tools/log4qt/consoleappender.cpp \
    Tools/log4qt/dailyrollingfileappender.cpp \
    Tools/log4qt/fileappender.cpp \
    Tools/log4qt/helpers/classlogger.cpp \
    Tools/log4qt/helpers/configuratorhelper.cpp \
    Tools/log4qt/helpers/datetime.cpp \
    Tools/log4qt/helpers/factory.cpp \
    Tools/log4qt/helpers/initialisationhelper.cpp \
    Tools/log4qt/helpers/logerror.cpp \
    Tools/log4qt/helpers/logobject.cpp \
    Tools/log4qt/helpers/logobjectptr.cpp \
    Tools/log4qt/helpers/optionconverter.cpp \
    Tools/log4qt/helpers/patternformatter.cpp \
    Tools/log4qt/helpers/properties.cpp \
    Tools/log4qt/hierarchy.cpp \
    Tools/log4qt/layout.cpp \
    Tools/log4qt/level.cpp \
    Tools/log4qt/log4qt.cpp \
    Tools/log4qt/logger.cpp \
    Tools/log4qt/loggerrepository.cpp \
    Tools/log4qt/loggingevent.cpp \
    Tools/log4qt/logmanager.cpp \
    Tools/log4qt/mdc.cpp \
    Tools/log4qt/ndc.cpp \
    Tools/log4qt/patternlayout.cpp \
    Tools/log4qt/propertyconfigurator.cpp \
    Tools/log4qt/rollingfileappender.cpp \
    Tools/log4qt/simplelayout.cpp \
    Tools/log4qt/spi/filter.cpp \
    Tools/log4qt/ttcclayout.cpp \
    Tools/log4qt/varia/debugappender.cpp \
    Tools/log4qt/varia/denyallfilter.cpp \
    Tools/log4qt/varia/levelmatchfilter.cpp \
    Tools/log4qt/varia/levelrangefilter.cpp \
    Tools/log4qt/varia/listappender.cpp \
    Tools/log4qt/varia/nullappender.cpp \
    Tools/log4qt/varia/stringmatchfilter.cpp \
    Tools/log4qt/writerappender.cpp \
    View/Common/ChatControl.cpp \
    View/Common/ChatMyWidget.cpp \
    View/Common/ChatWidget.cpp \
    View/Common/ChatYouWidget.cpp \
    View/Common/ImageTipsWidgetWidget.cpp \
    View/Common/RoundLoadingWidget.cpp \
    View/Common/WidBtn.cpp \
    View/Common/WidDialog.cpp \
    View/Home/WidHome.cpp \
    View/Login/WidLogin.cpp \
    View/Room/WidDiaplayView.cpp \
    View/Room/WidRoom.cpp \
    View/Room/WidSetData.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    CJsonObject/JsonUtil.h \
    DataModel/MemberDataModel.h \
    DataModel/RoomDataModel.h \
    Global/GlobalDataClass.h \
    NetWork/HttpNetwork.h \
    RtcSdk/SRTCControl.h \
    RtcSdk/VideoDevice.h \
    Tools/FlowLayout.h \
    Tools/MyScrollArea.h \
    Tools/SetImageClass.h \
    Tools/Utils.h \
    Tools/log4qt/appender.h \
    Tools/log4qt/appenderskeleton.h \
    Tools/log4qt/basicconfigurator.h \
    Tools/log4qt/consoleappender.h \
    Tools/log4qt/dailyrollingfileappender.h \
    Tools/log4qt/fileappender.h \
    Tools/log4qt/helpers/classlogger.h \
    Tools/log4qt/helpers/configuratorhelper.h \
    Tools/log4qt/helpers/datetime.h \
    Tools/log4qt/helpers/factory.h \
    Tools/log4qt/helpers/initialisationhelper.h \
    Tools/log4qt/helpers/logerror.h \
    Tools/log4qt/helpers/logobject.h \
    Tools/log4qt/helpers/logobjectptr.h \
    Tools/log4qt/helpers/optionconverter.h \
    Tools/log4qt/helpers/patternformatter.h \
    Tools/log4qt/helpers/properties.h \
    Tools/log4qt/hierarchy.h \
    Tools/log4qt/layout.h \
    Tools/log4qt/level.h \
    Tools/log4qt/log4qt.h \
    Tools/log4qt/logger.h \
    Tools/log4qt/loggerrepository.h \
    Tools/log4qt/loggingevent.h \
    Tools/log4qt/logmanager.h \
    Tools/log4qt/mdc.h \
    Tools/log4qt/ndc.h \
    Tools/log4qt/patternlayout.h \
    Tools/log4qt/propertyconfigurator.h \
    Tools/log4qt/rollingfileappender.h \
    Tools/log4qt/simplelayout.h \
    Tools/log4qt/spi/filter.h \
    Tools/log4qt/ttcclayout.h \
    Tools/log4qt/varia/debugappender.h \
    Tools/log4qt/varia/denyallfilter.h \
    Tools/log4qt/varia/levelmatchfilter.h \
    Tools/log4qt/varia/levelrangefilter.h \
    Tools/log4qt/varia/listappender.h \
    Tools/log4qt/varia/nullappender.h \
    Tools/log4qt/varia/stringmatchfilter.h \
    Tools/log4qt/writerappender.h \
    View/Common/ChatControl.h \
    View/Common/ChatMyWidget.h \
    View/Common/ChatWidget.h \
    View/Common/ChatYouWidget.h \
    View/Common/IChatBase.h \
    View/Common/ImageTipsWidgetWidget.h \
    View/Common/RoundLoadingWidget.h \
    View/Common/WidBtn.h \
    View/Common/WidDialog.h \
    View/Home/WidHome.h \
    View/Login/WidLogin.h \
    View/Room/WidDiaplayView.h \
    View/Room/WidRoom.h \
    View/Room/WidSetData.h \
    widget.h

FORMS += \
    View/Common/ChatMyWidget.ui \
    View/Common/ChatWidget.ui \
    View/Common/ChatYouWidget.ui \
    View/Common/ImageTipsWidgetWidget.ui \
    View/Common/WidBtn.ui \
    View/Common/WidDialog.ui \
    View/Home/WidHome.ui \
    View/Login/WidLogin.ui \
    View/Room/WidDiaplayView.ui \
    View/Room/WidRoom.ui \
    View/Room/WidSetData.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc

LIBS        += -L$$PWD/RtcSdk/lib -lRTCEngine
INCLUDEPATH += $$PWD/RtcSdk/include

INCLUDEPATH += $$PWD/Tools

LIBS += -lDbghelp
