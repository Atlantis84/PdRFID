#-------------------------------------------------
#
# Project created by QtCreator 2022-04-13T10:23:56
#
#-------------------------------------------------

QT += core gui sql network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PdRFID
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14
QMAKE_CXXFLAGS_RELEASE = -Od -ZI -MD
QMAKE_LFLAGS_RELEASE = /DEBUG /INCREMENTAL:NO

SOURCES += \
        StationClasses/stationbase.cpp \
        StationClasses/stationbrace.cpp \
        StationClasses/stationdiode.cpp \
        StationClasses/stationigbt.cpp \
        StationClasses/stationlineorder.cpp \
        StationClasses/stationonline.cpp \
        StationClasses/stationoutplate.cpp \
        StationClasses/stationpastelabel.cpp \
        StationClasses/stationpcba.cpp \
        StationClasses/stationradiator.cpp \
        StationClasses/stationsilicon.cpp \
        TcpClient/tcpsocket.cpp \
        TitleBar.cpp \
        centralwgt.cpp \
        datacommserial.cpp \
        detailinfowgt.cpp \
        gdatafactory.cpp \
        httpcpps/httpconnectionhandler.cpp \
        httpcpps/httpconnectionhandlerpool.cpp \
        httpcpps/httpcookie.cpp \
        httpcpps/httpglobal.cpp \
        httpcpps/httplistener.cpp \
        httpcpps/httprequest.cpp \
        httpcpps/httprequesthandler.cpp \
        httpcpps/httpresponse.cpp \
        httpcpps/httpsession.cpp \
        httpcpps/httpsessionstore.cpp \
        httpcpps/staticfilecontroller.cpp \
        httpcpps/template.cpp \
        httpcpps/templatecache.cpp \
        httpcpps/templateloader.cpp \
        imessagebox.cpp \
        labeldetailinfo.cpp \
        logowgt.cpp \
        main.cpp \
        mainwgt.cpp \
        mainwindow.cpp \
        realtimelogwgt.cpp \
        stationinfowgt.cpp \
        titlewidget.cpp \
        udpservice.cpp

HEADERS += \
        StationClasses/stationbase.h \
        StationClasses/stationbrace.h \
        StationClasses/stationdiode.h \
        StationClasses/stationigbt.h \
        StationClasses/stationlineorder.h \
        StationClasses/stationonline.h \
        StationClasses/stationoutplate.h \
        StationClasses/stationpastelabel.h \
        StationClasses/stationpcba.h \
        StationClasses/stationradiator.h \
        StationClasses/stationsilicon.h \
        TcpClient/tcpsocket.h \
        TitleBar.h \
        centralwgt.h \
        datacommserial.h \
        detailinfowgt.h \
        gdatafactory.h \
        httpheaders/httpconnectionhandler.h \
        httpheaders/httpconnectionhandlerpool.h \
        httpheaders/httpcookie.h \
        httpheaders/httpglobal.h \
        httpheaders/httplistener.h \
        httpheaders/httprequest.h \
        httpheaders/httprequesthandler.h \
        httpheaders/httpresponse.h \
        httpheaders/httpsession.h \
        httpheaders/httpsessionstore.h \
        httpheaders/staticfilecontroller.h \
        httpheaders/template.h \
        httpheaders/templatecache.h \
        httpheaders/templateglobal.h \
        httpheaders/templateloader.h \
        imessagebox.h \
        labeldetailinfo.h \
        logowgt.h \
        mainwgt.h \
        mainwindow.h \
        realtimelogwgt.h \
        stationinfowgt.h \
        titlewidget.h \
        udpservice.h

INCLUDEPATH += .\httpheaders

INCLUDEPATH += .\QsLog-master
INCLUDEPATH += .\DMDBAccess
INCLUDEPATH += .\StationClasses
INCLUDEPATH += .\TcpClient

DEFINES += QSLOG_IS_SHARED_LIBRARY_IMPORT
DEFINES += QS_LOG_LINE_NUMBERS

LIBS += -L$$PWD\lib\x64\ -lQsLog2
LIBS += -L$$PWD\lib\x64\ -lDMDBAccess

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES +=
