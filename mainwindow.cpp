#include "mainwindow.h"
#include <QApplication>
#include "TitleBar.h"
#include <QPainter>
#include <QStatusBar>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include "gdatafactory.h"
#include <QFile>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QHttpPart>
#include <QUuid>
#include <QNetworkReply>
#include <QFileInfo>
#include <QJsonParseError>
#include "imessagebox.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->setStyleSheet("QMainWindow{background-color:#000000}");

    createStatusBar();
    createActions();
    this->setCentralWidget(GDataFactory::get_central_wgt());
    m_pMonitorCount = 0;
}

MainWindow::~MainWindow()
{

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.fillRect(this->rect(),QBrush(QColor(240,242,245)));
    return;
}

int testC=0;
void MainWindow::slot_calibration()
{
//    if(testC == 0)
//        upload_file("D://1.png");
//    else if(testC == 1)
//        upload_file("f://8921_GD.xml");
//    testC++;

//    if(testC >7)
//        ;
//    else
//        GDataFactory::get_factory()->read_serial_number_xray(testC);
//    testC++;
//    return;

    QString tmpurl;
    tmpurl = "http://";
    tmpurl.append("172.16.123.74");
    tmpurl.append(":");
    tmpurl.append("8095");
    tmpurl.append("/InfoWebs.asmx/InfoApi");
    QNetworkReply* reply;
    QEventLoop eventloop;
    QNetworkRequest* request = new QNetworkRequest();
    request->setUrl(QUrl(tmpurl));
    QString strPos = u8"一车间点料机";
    QString reData = "{\"TYPE\":\"X\",\"WORKSTATION\":\"";
    reData.append(strPos);
    reData.append("\",\"EMP\":\"*****\",\"SN\":\"36958-1053134-20AN-20300\",\"QTY\":\"10000\"}");
    QString reType = QString::fromUtf8("application/x-www-form-urlencoded");
    request->setHeader(QNetworkRequest::ContentTypeHeader,QVariant(reType));

    QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
    QString rInfo = "info=";
    rInfo.append(reData);
    reply = accessManager->post(*request,rInfo.toUtf8());
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    //set get request time out
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        QByteArray responseByte = reply->readAll();
        QString tmpData = QString::fromUtf8(responseByte);
        int index1 = tmpData.indexOf("{");
        int index2 = tmpData.indexOf("}");
        int indexLength = index2 - index1 + 1;
        tmpData = tmpData.mid(index1,indexLength);
        QJsonDocument jsonDocument = QJsonDocument::fromJson(tmpData.toUtf8());
        QJsonObject jsonObject = jsonDocument.object();
        QLOG_INFO()<<"serial port http get request reply is NORMAL!";
    }
    else
    {
        QLOG_WARN()<<"serial port http get request reply is TIMEOUT!";;
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerId)
    {
        GDataFactory::get_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D1010,20,nullptr);
        killTimer(m_pTimerId);
    }
    else if(event->timerId() == m_pMonitorTimerId)
    {
        m_pMonitorCount++;
        if(m_pMonitorCount ==5)
        {
            IMessageBox* msgBox = new IMessageBox(3);
            msgBox->warning(u8"PLC读返回超时!");
            m_pMonitorCount = 0;
            killTimer(m_pMonitorTimerId);
        }
    }
    else
        ;
//    QNetworkRequest request;
//    request.setUrl(QUrl("http://127.0.0.1:8080"));
//    request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));

//    QJsonObject reJson;
//    reJson.insert("reType","PD_TEST");
//    reJson.insert("reLineName","T104");
//    reJson.insert("reStationName",u8"机台1");
//    QJsonDocument doc_data1(reJson);
//    QByteArray response_data = doc_data1.toJson(QJsonDocument::Compact);

//    QNetworkAccessManager* tmpaccessManager = new QNetworkAccessManager(this);
//    QNetworkReply *reply=tmpaccessManager->post(request,response_data);
//    QEventLoop eventloop;
//    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
//    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
//    eventloop.exec();

//    if(reply->isFinished())
//    {
//        if (reply->error() == QNetworkReply::NoError){
//            QLOG_INFO()<<reply->readAll();
//            download_file("8921.xml");
//            QTimer::singleShot(2000,&eventloop,&QEventLoop::quit);
//            eventloop.exec();
//            download_file("8921_GD.xml");

//        }
//    }
}

void MainWindow::upload_file(QString filewithpath)
{
    QFile *file = new QFile(filewithpath);
    QFileInfo fileInfo(filewithpath);
    if(!file->open(QIODevice::ReadOnly))
        qDebug()<<"open file failed!";

    //file upload procedure
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:8080/upload_img"));

    QHttpPart filePart;
    QString requestFormat = QString::fromUtf8("form-data; name=\"upload_file\"; filename=\"%1\"").arg(fileInfo.fileName());
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(requestFormat));

    QString rt = "OK";
    QHttpPart filePart1;
    QString requestFormat1 = QString::fromUtf8("form-data; name=\"vision_result\"");
    filePart1.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant(requestFormat1));
    filePart1.setBody(rt.toLatin1());
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    filePart.setBodyDevice(file);
    multiPart->append(filePart);
    multiPart->append(filePart1);

    QNetworkAccessManager* tmpaccessManager = new QNetworkAccessManager(this);
    QNetworkReply *reply=tmpaccessManager->post(request,multiPart);
    QEventLoop eventloop;
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QTimer::singleShot(10000,&eventloop,&QEventLoop::quit);
    eventloop.exec();

    if(reply->isFinished())
    {
        if (reply->error() == QNetworkReply::NoError){
            QByteArray responseData = reply->readAll();
            if(QString(responseData) == "SUCCESS_PD")
                qDebug()<<"upload file success";
            else {
                qDebug()<<"upload file failed";
            }
        }
    }
    else{
        qDebug()<<"upload file timeout";
    }
    //file upload procedure
}

void MainWindow::slot_start_run()
{
    m_pTimerId = startTimer(1000);
    m_pMonitorTimerId = startTimer(1000);
//    GDataFactory::get_udp_service()->send_message_to_plc(READ_PLC,ADDRESS_D1010,20,nullptr);
//    download_file("1.tif");
    //    GDataFactory::get_factory()->read_serial_number();
}

void MainWindow::download_file(QString filename)
{
    QNetworkRequest request;
    request.setUrl(QUrl("http://127.0.0.1:8080/download"));
    request.setRawHeader("download_file",filename.toLatin1());

    QNetworkAccessManager* tmpaccessManager = new QNetworkAccessManager(this);
    QNetworkReply *reply=tmpaccessManager->get(request);
    QEventLoop eventloop;
    connect(reply,SIGNAL(finished()),&eventloop,SLOT(quit()));
    QTimer::singleShot(5000,&eventloop,&QEventLoop::quit);
    eventloop.exec();
    if(reply->isFinished())
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            QList<QByteArray> tmpLst = reply->rawHeaderList();
            if(tmpLst.length() == 2)
            {
//                for (int var = 0; var < tmpLst.length(); ++var) {
//                    qDebug()<<reply->rawHeader(tmpLst[var]);
//                }
                if(reply->rawHeader(tmpLst[1]) == "SUCCESS_PD")
                {
                    qDebug()<<"download file SUCCESS";
                    QByteArray responseByte = reply->readAll();
                    QString tmpfName = "d:/";
                    tmpfName.append(filename);
                    QFile writeFile(tmpfName);
                    writeFile.open(QIODevice::ReadWrite);
                    writeFile.write(responseByte);
                    writeFile.close();
                }
                else {
                    qDebug()<<"download file FAILED";
                }
            }
            else {
                qDebug()<<"upload procedure response-header exist ERROR";
            }
        }
    }
}

void MainWindow::createStatusBar()
{
    QFont font("LiSu", 18, QFont::StyleNormal);
    statusBar()->setFont(font);
    statusBar()->setStyleSheet("QStatusBar{color:rgba(255,255,255,150);border:1px solid rgb(255,255,255,100);"
                               "background-color:rgba(216,217,222,0);}");
    statusBar()->showMessage(u8"MISSION FREE");
    QLabel *lb1 = new QLabel(u8"Hisense|",this);
    lb1->setStyleSheet("font-family:\"LiSu\";color:rgba(255,255,255,150);");
    QLabel *lb2 = new QLabel(u8"Copyright@2022Hisense_ZDJG",this);
    lb2->setStyleSheet("font-family:\"LiSu\";color:rgba(255,255,255,150);");
    lb1->setFont(font);
    lb2->setFont(font);
    statusBar()->addPermanentWidget(lb1);
    statusBar()->addPermanentWidget(lb2);
}

void MainWindow::createActions()
{
    QWidget *wtSpace[10];
    for (int i=0;i<10;i++) {
        wtSpace[i] = new QWidget();
        wtSpace[i]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    }

    QToolBar *fileToolBar = new QToolBar();
    this->addToolBar(Qt::RightToolBarArea,fileToolBar);
    fileToolBar->setStyleSheet("QToolBar{border:1px solid rgba(255,255,255,100);"
                               "background-color:rgba(235,236,240,0);}"
                               "QToolButton{background-color:rgba(22,194,194,0);"
                               "color:rgb(0,0,0);}");
    const QIcon userIcon = QIcon::fromTheme("document-new", QIcon(":/icon/user.png"));
    const QIcon startIcon = QIcon::fromTheme("",QIcon(":/icon/start.png"));
    const QIcon userManageIcon = QIcon::fromTheme("document-save", QIcon(":/icon/user_manage.png"));

    QAction *calibrationAct = new QAction(userIcon, u8"", this);
    QAction *startRunAct = new QAction(startIcon, u8"",this);
    QAction *stopRunAct = new QAction(userManageIcon, u8"", this);
    calibrationAct->setStatusTip(u8"Station_1");
    startRunAct->setStatusTip(u8"Station_2");
    stopRunAct->setStatusTip(u8"Station_3");
    connect(stopRunAct,SIGNAL(triggered()), this, SLOT(slot_stop_run()));
    connect(calibrationAct,SIGNAL(triggered()), this, SLOT(slot_calibration()));
    connect(startRunAct,SIGNAL(triggered()), this, SLOT(slot_start_run()));
    fileToolBar->addAction(calibrationAct);
    fileToolBar->addAction(startRunAct);
    fileToolBar->addAction(stopRunAct);
//    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    fileToolBar->setIconSize(QSize(30,30));

    QAction *quitAct = new QAction(u8"&退出",this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip(u8"退出应用程序");
    connect(quitAct,SIGNAL(triggered()),this,SLOT(exitApp()));

    const QIcon boardNumberIcon = QIcon::fromTheme("", QIcon(":/icon/order_load.ico"));
    const QIcon portSetIcon = QIcon::fromTheme("",QIcon(":/icon/order_edit.ico"));

    QAction *historyCalibrationAct = new QAction(boardNumberIcon,u8"",this);
    QAction *portSetAct = new QAction(portSetIcon,u8"",this);
    QLabel* hisenseLogo = new QLabel();
    hisenseLogo->setStyleSheet("QLabel{min-width:350px;min-height:80px;background-color:rgba(0,0,0,0);border:none;}");
    QPixmap *hisensePixmap = new QPixmap(":/icon/2.png");
    hisensePixmap->scaled(hisenseLogo->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    hisenseLogo->setPixmap(*hisensePixmap);

    historyCalibrationAct->setStatusTip(u8"Station_4");
    connect(historyCalibrationAct,SIGNAL(triggered()), this, SLOT(slot_history_calibration()));
    portSetAct->setStatusTip(u8"Station_5");
    connect(portSetAct,SIGNAL(triggered()), this, SLOT(slot_para_set()));

    fileToolBar->addAction(historyCalibrationAct);
    fileToolBar->addAction(portSetAct);
//    fileToolBar->addWidget(wtSpace[3]);
//    fileToolBar->addWidget(hisenseLogo);

    QAction *aboutAct = new QAction(u8"&关于",this);
    aboutAct->setStatusTip(u8"显示应用程序关于对话框");
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(about()));
}

void MainWindow::slot_proc_station_info_from_plc(const QByteArray data)
{
    if(data.length() != 20)
    {
        QLOG_WARN()<<"the station info from plc is ABNORMAL!";
        return;
    }
    m_pMonitorCount = 0;

    if(data[0] | data[1])
    {
        QLOG_INFO()<<"on line RFID scan code";//on line
        GDataFactory::get_tcp_socket()->send_to_front_server("0");
    }
    if(data[2] | data[3])
    {
        QLOG_INFO()<<"silicon RFID scan code";//silicon
        GDataFactory::get_tcp_socket()->send_to_front_server("1");
    }
    if(data[4] | data[5])
    {
        QLOG_INFO()<<"radiator RFID scan code";//radiator
        GDataFactory::get_tcp_socket()->send_to_front_server("2");
    }
    if(data[6] | data[7])
    {
        QLOG_INFO()<<"brace RFID scan code";//brace
        GDataFactory::get_tcp_socket()->send_to_middle_server("3");
    }
    if(data[8] | data[9])
    {
        QLOG_INFO()<<"PCBA RFID scan code";//PCBA
        GDataFactory::get_tcp_socket()->send_to_middle_server("4");
    }
    if(data[10] | data[11])
    {
        QLOG_INFO()<<"diode RFID scan code";//diode
        GDataFactory::get_tcp_socket()->send_to_middle_server("5");
    }
    if(data[12] | data[13])
    {
        QLOG_INFO()<<"IGBT RFID scan code";//IGBT
        GDataFactory::get_tcp_socket()->send_to_back_server("6");
    }
    if(data[14] | data[15])
    {
        QLOG_INFO()<<"out plate RFID scan code";//out plate
        GDataFactory::get_tcp_socket()->send_to_back_server("7");
    }
    if(data[16] | data[17])
    {
        QLOG_INFO()<<"line order RFID scan code";//line order
        GDataFactory::get_tcp_socket()->send_to_back_server("8");
    }

    QByteArray clearArray;
    for(int i=0;i<18;i++)
        clearArray.append(static_cast<char>(0x00));
    GDataFactory::get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D1010,18,clearArray);

    if(data[18] | data[19])
    {
        QLOG_INFO()<<"paste label RFID scan code";//paste label
        GDataFactory::get_tcp_socket()->send_to_back_server("9");
    }
    else
    {
        m_pTimerId = startTimer(1000);
    }
}
