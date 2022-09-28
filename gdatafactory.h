#ifndef GDATAFACTORY_H
#define GDATAFACTORY_H

#include <QObject>
#include "mainwgt.h"
#include "mainwindow.h"
#include "dmdbaccessinterface.h"
#include "centralwgt.h"
#include "detailinfowgt.h"
#include "QsLog.h"
#include "stationradiator.h"
#include "stationbrace.h"
#include "stationdiode.h"
#include "stationigbt.h"
#include "stationlineorder.h"
#include "stationonline.h"
#include "stationoutplate.h"
#include "stationpastelabel.h"
#include "stationpcba.h"
#include "stationsilicon.h"
#include "realtimelogwgt.h"
#include "stationinfowgt.h"
#include "logowgt.h"
#include "labeldetailinfo.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMap>
#include "tcpsocket.h"
#include "datacommserial.h"
#include "udpservice.h"

#define ADDRESS_D1010 0x03F2    //online
#define ADDRESS_D1011 0x03F3    //silicon
#define ADDRESS_D1012 0x03F4    //radiator
#define ADDRESS_D1013 0x03F5    //brace
#define ADDRESS_D1014 0x03F6    //PCBA
#define ADDRESS_D1015 0x03F7    //diode
#define ADDRESS_D1016 0x03F8    //IGBT
#define ADDRESS_D1017 0x03F9    //outplate
#define ADDRESS_D1018 0x03FA    //lineorder
#define ADDRESS_D1019 0x03FB    //pastelabel
#define READ_PLC  0x01 //function code of read plc
#define WRITE_PLC 0x02 //function code of write plc

class GDataFactory : public QObject
{
    Q_OBJECT
public:
    explicit GDataFactory(QObject *parent = nullptr);

    void load_json_config(char* filename);
    QString get_config_para(const QString keyname);
    void set_config_para(const QString keyname, const QString keyvalue);
    void save_config_file();
    void connect_common_signal_slot();

    void load_pre_station_info();
    QList<StationBase*> get_station_list()
    {return m_pStationList;}
    void init_serial_port();
    void read_serial_number();
    void read_serial_number_xray(int functioncode);
    quint8 get_last_ip(const int id);
    quint8 process_ip_string(const QString ipstr);
private:
    QFile m_pConfigFile;
    QJsonDocument m_pJsonDocument;
    QJsonObject m_pRootObj;
    QString m_pConfigFileName;
    QMap<QString,QString> ConfigInfo;
    int get_biggest_id(const QString &tablename);
    QList<StationBase*> m_pStationList;
    QMutex m_pDBMutex;
    DataCommSerial* pComm;
    QString m_pCurrentLabelCode;//RFID label code
    QString m_pCurrentQrCode;//the code of the end of the line

    void timerEvent(QTimerEvent *event);
    int m_timer_id;
    int m_read_serial_count;
    void clear_station_info_and_post_to_mes();

public:
    static GDataFactory* get_factory()
    {
        if(m_pDataFactory == nullptr)
            m_pDataFactory = new GDataFactory();
        return m_pDataFactory;
    }

    static MainWgt* get_main_wgt()
    {
        if(m_pMainWgt == nullptr)
            m_pMainWgt = new MainWgt();
        return m_pMainWgt;
    }

    static MainWindow* get_main_window()
    {
        if(m_pMainWindow == nullptr)
            m_pMainWindow = new MainWindow();
        return m_pMainWindow;
    }

    static DMDBAccessInterface* get_pgsql()
    {
        if(m_pDMDBAccess == nullptr)
            m_pDMDBAccess = DMDBAccessInterface::GetInterface() ;
        return m_pDMDBAccess;
    }

    static CentralWgt* get_central_wgt()
    {
        if(m_pCentralWgt == nullptr)
            m_pCentralWgt = new CentralWgt();
        return m_pCentralWgt;
    }

    static DetailInfoWgt* get_detail_info_wgt()
    {
        if(m_pDetailInfoWgt == nullptr)
            m_pDetailInfoWgt = new DetailInfoWgt();
        return m_pDetailInfoWgt;
    }

    static StationBrace* get_station_brace()
    {
        if(m_pStationBrace == nullptr)
            m_pStationBrace = new StationBrace();
        return m_pStationBrace;
    }

    static StationDiode* get_station_diode()
    {
        if(m_pStationDiode == nullptr)
            m_pStationDiode = new StationDiode();
        return m_pStationDiode;
    }

    static StationIGBT* get_station_IGBT()
    {
        if(m_pStationIGBT == nullptr)
            m_pStationIGBT = new StationIGBT();
        return m_pStationIGBT;
    }

    static StationLineOrder* get_station_line_order()
    {
        if(m_pStationLineOrder == nullptr)
            m_pStationLineOrder = new StationLineOrder();
        return m_pStationLineOrder;
    }

    static StationOnLine* get_station_on_line()
    {
        if(m_pStationOnLine == nullptr)
            m_pStationOnLine = new StationOnLine();
        return m_pStationOnLine;
    }

    static StationOutPlate* get_station_out_plate()
    {
        if(m_pStationOutPlate == nullptr)
            m_pStationOutPlate = new StationOutPlate();
        return m_pStationOutPlate;
    }

    static StationPasteLabel* get_station_paste_label()
    {
        if(m_pStationPasteLabel == nullptr)
            m_pStationPasteLabel = new StationPasteLabel();
        return m_pStationPasteLabel;
    }

    static StationPCBA* get_station_PCBA()
    {
        if(m_pStationPCBA == nullptr)
            m_pStationPCBA = new StationPCBA();
        return m_pStationPCBA;
    }

    static StationRadiator* get_station_radiator()
    {
        if(m_pStationRadiator == nullptr)
            m_pStationRadiator = new StationRadiator();
        return m_pStationRadiator;
    }

    static StationSilicon* get_station_silicon()
    {
        if(m_pStationSilicon == nullptr)
            m_pStationSilicon = new StationSilicon();
        return m_pStationSilicon;
    }

    static RealTimeLogWgt* get_real_time_log_wgt()
    {
        if(m_pRealTimeLogWgt == nullptr)
            m_pRealTimeLogWgt = new RealTimeLogWgt();
        return m_pRealTimeLogWgt;
    }

    static StationInfoWgt* get_station_info_wgt()
    {
        if(m_pStationInfoWgt == nullptr)
            m_pStationInfoWgt = new StationInfoWgt();
        return m_pStationInfoWgt;
    }

    static LogoWgt* get_logo_wgt()
    {
        if(m_pLogoWgt == nullptr)
            m_pLogoWgt = new LogoWgt();
        return m_pLogoWgt;
    }

    static LabelDetailInfo* get_label_detail_info_wgt()
    {
        if(m_pLabelDetailInfoWgt == nullptr)
            m_pLabelDetailInfoWgt = new LabelDetailInfo();
        return m_pLabelDetailInfoWgt;
    }

    static TcpSocket* get_tcp_socket()
    {
        if(m_pTcpSocket == nullptr)
            m_pTcpSocket = new TcpSocket();
        return m_pTcpSocket;
    }

    static UdpService* get_udp_service()
    {
        if(m_pUdpService == nullptr)
            m_pUdpService = new UdpService();
        return m_pUdpService;
    }

    QString bytes_to_str(QByteArray data);
signals:

public slots:
    void slot_notify_update_db(STATIONTYPE sType,QString lCode);
    void slot_notify_update_paste_label_db(QString lCode);
    void slot_rev_serial_number(const QByteArray&);

private:
    static GDataFactory* m_pDataFactory;
    static MainWgt* m_pMainWgt;
    static MainWindow* m_pMainWindow;
    static DMDBAccessInterface* m_pDMDBAccess;
    static CentralWgt* m_pCentralWgt;
    static DetailInfoWgt* m_pDetailInfoWgt;

    static StationBrace* m_pStationBrace;
    static StationDiode* m_pStationDiode;
    static StationIGBT* m_pStationIGBT;
    static StationLineOrder* m_pStationLineOrder;
    static StationOnLine* m_pStationOnLine;
    static StationOutPlate* m_pStationOutPlate;
    static StationPasteLabel* m_pStationPasteLabel;
    static StationPCBA* m_pStationPCBA;
    static StationRadiator* m_pStationRadiator;
    static StationSilicon* m_pStationSilicon;
    static RealTimeLogWgt* m_pRealTimeLogWgt;
    static StationInfoWgt* m_pStationInfoWgt;
    static LogoWgt* m_pLogoWgt;
    static LabelDetailInfo* m_pLabelDetailInfoWgt;
    static TcpSocket* m_pTcpSocket;
    static UdpService* m_pUdpService;
};

#endif // GDATAFACTORY_H
