#include "gdatafactory.h"
#include <QSqlQuery>
#include <QEventLoop>
#include <QTimer>
#include <QDateTime>
#include <QHostInfo>
GDataFactory* GDataFactory::m_pDataFactory=nullptr;
MainWgt* GDataFactory::m_pMainWgt=nullptr;
MainWindow* GDataFactory::m_pMainWindow=nullptr;
DMDBAccessInterface* GDataFactory::m_pDMDBAccess = nullptr;
CentralWgt* GDataFactory::m_pCentralWgt = nullptr;
DetailInfoWgt* GDataFactory::m_pDetailInfoWgt = nullptr;

StationBrace* GDataFactory::m_pStationBrace = nullptr;
StationDiode* GDataFactory::m_pStationDiode = nullptr;
StationIGBT* GDataFactory::m_pStationIGBT = nullptr;
StationLineOrder* GDataFactory::m_pStationLineOrder = nullptr;
StationOnLine* GDataFactory::m_pStationOnLine = nullptr;
StationOutPlate* GDataFactory::m_pStationOutPlate = nullptr;
StationPasteLabel* GDataFactory::m_pStationPasteLabel = nullptr;
StationPCBA* GDataFactory::m_pStationPCBA = nullptr;
StationRadiator* GDataFactory::m_pStationRadiator = nullptr;
StationSilicon* GDataFactory::m_pStationSilicon = nullptr;
RealTimeLogWgt* GDataFactory::m_pRealTimeLogWgt = nullptr;
StationInfoWgt* GDataFactory::m_pStationInfoWgt = nullptr;
LogoWgt* GDataFactory::m_pLogoWgt = nullptr;
LabelDetailInfo* GDataFactory::m_pLabelDetailInfoWgt = nullptr;
TcpSocket* GDataFactory::m_pTcpSocket = nullptr;
UdpService* GDataFactory::m_pUdpService = nullptr;
GDataFactory::GDataFactory(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<StationBase*>("StationBase*");
    m_pStationList.push_back(get_station_brace());
    m_pStationList.push_back(get_station_diode());
    m_pStationList.push_back(get_station_IGBT());
    m_pStationList.push_back(get_station_line_order());
    m_pStationList.push_back(get_station_on_line());
    m_pStationList.push_back(get_station_out_plate());
    m_pStationList.push_back(get_station_PCBA());
    m_pStationList.push_back(get_station_radiator());
    m_pStationList.push_back(get_station_silicon());
    pComm = nullptr;
    m_read_serial_count = 0;
}

void GDataFactory::init_serial_port()
{

}

void GDataFactory::read_serial_number()
{
    if(pComm)
    {
//        pComm->quit();
        delete pComm;
        pComm = nullptr;
    }

    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = ConfigInfo["COM_PORT"];
        QString portRate = ConfigInfo["COM_BARD_RATE"];
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));

        QList<QByteArray> datas;
        QByteArray aa;
        aa.append(static_cast<char>(0x04));
        aa.append(static_cast<char>(0xE4));
        aa.append(static_cast<char>(0x04));
        aa.append(static_cast<char>(0x00));
        aa.append(static_cast<char>(0xFF));
        aa.append(static_cast<char>(0x14));
        datas.append(aa);
        pComm->sendRequest_and_recvResponse(datas,500);
        QThread qThread;
        pComm->moveToThread(&qThread);
        pComm->run();
    }
}

void GDataFactory::read_serial_number_xray(int functioncode)
{
    if(pComm)
    {
//        pComm->quit();
        delete pComm;
        pComm = nullptr;
    }

    pComm = new DataCommSerial();
    connect(pComm,SIGNAL(sigRecvResponse(const QByteArray&)),this,SLOT(slot_rev_serial_number(const QByteArray&)));
    if(pComm)
    {
        QString portName = ConfigInfo["COM_PORT"];
        QString portRate = ConfigInfo["COM_BARD_RATE"];
        pComm->init(tagSerialPortInfo(portName,portRate.toInt()));

        QList<QByteArray> datas;
        QByteArray aa;
        switch (functioncode) {
        case 0://FREV
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x46));
            aa.append(static_cast<char>(0x52));
            aa.append(static_cast<char>(0x45));
            aa.append(static_cast<char>(0x56));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"FREV";
            break;
        case 1://watch dog
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x57));
            aa.append(static_cast<char>(0x44));
            aa.append(static_cast<char>(0x4F));
            aa.append(static_cast<char>(0x47));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"watch dog";
            break;
        case 2://clr
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x43));
            aa.append(static_cast<char>(0x4C));
            aa.append(static_cast<char>(0x52));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"CLR";
            break;
        case 3://enable
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x45));
            aa.append(static_cast<char>(0x4E));
            aa.append(static_cast<char>(0x42));
            aa.append(static_cast<char>(0x4C));
            aa.append(static_cast<char>(0x31));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"ENABLE";
            break;
        case 4://on
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x53));
            aa.append(static_cast<char>(0x54));
            aa.append(static_cast<char>(0x41));
            aa.append(static_cast<char>(0x52));
            aa.append(static_cast<char>(0x54));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"ON";
            break;
        case 5://vp
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x56));
            aa.append(static_cast<char>(0x50));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x35));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"VP";
            break;
        case 6://cp
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x43));
            aa.append(static_cast<char>(0x50));
            aa.append(static_cast<char>(0x34));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"CP";
            break;
        case 7://unable
            aa.append(static_cast<char>(0x02));
            aa.append(static_cast<char>(0x45));
            aa.append(static_cast<char>(0x4E));
            aa.append(static_cast<char>(0x42));
            aa.append(static_cast<char>(0x4C));
            aa.append(static_cast<char>(0x30));
            aa.append(static_cast<char>(0x0D));
            aa.append(static_cast<char>('\0'));
            QLOG_INFO()<<"UNABLE";
            break;
        default:
            break;
        }

        datas.append(aa);
        pComm->sendRequest_and_recvResponse(datas,500);
        QThread qThread;
        pComm->moveToThread(&qThread);
        pComm->run();
    }
}

void GDataFactory::load_json_config(char *filename)
{
    m_pConfigFile.setFileName(QString(filename));
    m_pConfigFileName=QString(filename);
    QString JsonValue;
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QLOG_INFO()<<"load json config file success!";
        JsonValue = m_pConfigFile.readAll();
        m_pConfigFile.close();

        QJsonParseError parseJsonErr;
        m_pJsonDocument = QJsonDocument::fromJson(JsonValue.toUtf8(),&parseJsonErr);
        if(!(parseJsonErr.error == QJsonParseError::NoError))
        {
            QLOG_INFO()<<"parse json file failed!";
            return;
        }
        else {
            QLOG_INFO()<<"parse json file success!";
        }

        m_pRootObj = m_pJsonDocument.object();
        if(m_pRootObj.contains("IPConfigs"))
        {
            QJsonValue arrayValue = m_pRootObj.value(QStringLiteral("IPConfigs"));
            if(arrayValue.isArray())
            {
                QJsonArray array = arrayValue.toArray();
                for (int i=0;i<array.size();i++)
                {
                    QJsonValue iconArray = array.at(i);
                    QJsonObject icon = iconArray.toObject();
                    ConfigInfo.insert("FRONT_RWRITE_IP",icon["FRONT_RWRITE_IP"].toString());
                    ConfigInfo.insert("MIDDLE_RWRITE_IP",icon["MIDDLE_RWRITE_IP"].toString());
                    ConfigInfo.insert("BACK_RWRITE_IP",icon["BACK_RWRITE_IP"].toString());
                    ConfigInfo.insert("COM_PORT",icon["COM_PORT"].toString());
                    ConfigInfo.insert("PLC_IP",icon["PLC_IP"].toString());
                }
            }
        }

        if(m_pRootObj.contains("PortConfigs"))
        {
            QJsonValue portValue = m_pRootObj.value(QStringLiteral("PortConfigs"));
            if(portValue.isObject())
            {
                QJsonObject obj = portValue.toObject();
                if(obj.contains("FRONT_RWRITE_PORT"))
                    ConfigInfo.insert("FRONT_RWRITE_PORT",obj.value("FRONT_RWRITE_PORT").toString());
                if(obj.contains("MIDDLE_RWRITE_PORT"))
                    ConfigInfo.insert("MIDDLE_RWRITE_PORT",obj.value("MIDDLE_RWRITE_PORT").toString());
                if(obj.contains("BACK_RWRITE_PORT"))
                    ConfigInfo.insert("BACK_RWRITE_PORT",obj.value("BACK_RWRITE_PORT").toString());
                if(obj.contains("COM_BARD_RATE"))
                    ConfigInfo.insert("COM_BARD_RATE",obj.value("COM_BARD_RATE").toString());
                if(obj.contains("PLC_PORT"))
                    ConfigInfo.insert("PLC_PORT",obj.value("PLC_PORT").toString());
            }
        }
    }
    else
    {
        QLOG_INFO()<<"load json config file failed!";
        return;
    }
    m_pConfigFile.flush();
}

QString GDataFactory::get_config_para(const QString keyname)
{
    return ConfigInfo[keyname];
}

void GDataFactory::set_config_para(const QString keyname, const QString keyvalue)
{
    ConfigInfo[keyname] = keyvalue;//update the config file,at the same time you have to update the info that has been loaded

    if(keyname == "FRONT_RWRITE_PORT" ||
            keyname == "MIDDLE_RWRITE_PORT" ||
            keyname == "BACK_RWRITE_PORT")
    {
        if(m_pRootObj.contains("PortConfigs") && m_pRootObj["PortConfigs"].isObject())
        {
            QJsonValueRef refValue = m_pRootObj.find("PortConfigs").value();
            QJsonObject obj = refValue.toObject();
            obj[keyname] = keyvalue;
            refValue = obj;
        }
    }

    if((keyname == "FRONT_RWRITE_IP") ||
            (keyname == "MIDDLE_RWRITE_IP") ||
            (keyname == "BACK_RWRITE_IP"))
    {
        QJsonValueRef refValue = m_pRootObj.find("IPConfigs").value();
        QJsonArray refArray = refValue.toArray();

        QJsonArray::Iterator arrayIterator = refArray.begin();
        QJsonValueRef elementOneValueRef = arrayIterator[0];
        QJsonObject elementOneObject = elementOneValueRef.toObject();
        elementOneObject.insert(keyname,keyvalue);
        elementOneValueRef = elementOneObject;
        refValue = refArray;
    }

    m_pJsonDocument.setObject(m_pRootObj);
}

void GDataFactory::save_config_file()
{
    m_pConfigFile.setFileName(m_pConfigFileName);
    if(m_pConfigFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        m_pConfigFile.resize(0);//this sentence is essential,or the file will be abnormal---20211123lilei
        m_pJsonDocument.setObject(m_pRootObj);
        m_pConfigFile.seek(0);
        m_pConfigFile.write(m_pJsonDocument.toJson());
        m_pConfigFile.close();
        m_pConfigFile.flush();
    }
}

void GDataFactory::connect_common_signal_slot()
{
    connect(get_udp_service(),SIGNAL(signal_proc_station_info_from_plc(QByteArray)),get_main_window(),SLOT(slot_proc_station_info_from_plc(QByteArray)));
    connect(get_station_on_line(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_on_line(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_brace(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_brace(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_diode(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_diode(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_IGBT(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_IGBT(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_line_order(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_line_order(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_out_plate(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_out_plate(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_PCBA(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_PCBA(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_radiator(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_radiator(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_silicon(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_silicon(),SIGNAL(signal_notify_update_db(STATIONTYPE,QString)),this,SLOT(slot_notify_update_db(STATIONTYPE,QString)));
    connect(get_station_paste_label(),SIGNAL(signal_notify_station_info(StationBase*)),get_station_info_wgt(),SLOT(slot_notify_station_info(StationBase*)));
    connect(get_station_paste_label(),SIGNAL(signal_notify_update_paste_label_db(QString)),this,SLOT(slot_notify_update_paste_label_db(QString)));
}

void GDataFactory::load_pre_station_info()
{
    QString strSql = QString("select * from public.%1").arg(constStationInfoTable);
    QSqlQuery queryResult;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        while(queryResult.next())
        {
            if(queryResult.value(1).toString() != "")
                get_station_on_line()->pre_add_label_code(queryResult.value(1).toString());
            else if(queryResult.value(2).toString() != "")
                get_station_silicon()->pre_add_label_code(queryResult.value(2).toString());
            else if(queryResult.value(3).toString() != "")
                get_station_radiator()->pre_add_label_code(queryResult.value(3).toString());
            else if(queryResult.value(4).toString() != "")
                get_station_brace()->pre_add_label_code(queryResult.value(4).toString());
            else if(queryResult.value(5).toString() != "")
                get_station_PCBA()->pre_add_label_code(queryResult.value(5).toString());
            else if(queryResult.value(6).toString() != "")
                get_station_diode()->pre_add_label_code(queryResult.value(6).toString());
            else if(queryResult.value(7).toString() != "")
                get_station_IGBT()->pre_add_label_code(queryResult.value(7).toString());
            else if(queryResult.value(8).toString() != "")
                get_station_out_plate()->pre_add_label_code(queryResult.value(8).toString());
            else if(queryResult.value(9).toString() != "")
                get_station_line_order()->pre_add_label_code(queryResult.value(9).toString());
            else {
                ;
            }
        }
    }

    strSql = QString("select * from public.%1").arg(constStationPasteLabelInfoTable);
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        while(queryResult.next())
        {
            if(queryResult.value(0).toString() != "")
                get_station_paste_label()->pre_add_label_code(queryResult.value(0).toString());
            else {
                ;
            }
        }
    }
}

QString GDataFactory::bytes_to_str(QByteArray data)
{
    QString str_data;
    str_data.clear();
    quint8 n_data = 0;
    for(int i=0; i<data.size(); i++)
    {
        n_data = static_cast<quint8>(data[i]) ;
        if(n_data < 16) str_data.append('0');
        str_data.append(QString::number(n_data, 16));
        str_data.append(' ');
    }
    return str_data;
}

int GDataFactory::get_biggest_id(const QString &tablename)
{
    QString strSql = QString("select * from public.%1 order by \"info_id\" desc").arg(tablename);
    QSqlQuery queryResult;
    int id = -1;
    if(GDataFactory::get_pgsql()->GetQueryResult(strSql,queryResult))
    {
        while(queryResult.next())
        {
            if(id > queryResult.value(0).toInt())
                continue;
            else
                id = queryResult.value(0).toInt();
        }
        if(id == -1)
        {
            id = 1;
            return id;
        }
        else
        {
            id += 1;
            return id;
        }
    }
    else
    {
        QLOG_WARN()<<u8"query database failed!";
        return -1;
    }
}

void GDataFactory::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timer_id)
    {
        if(m_pCurrentQrCode == "")
        {
            if(m_read_serial_count == 3)
            {
                killTimer(m_timer_id);
                m_read_serial_count = 0;
            }
            read_serial_number();
            m_read_serial_count++;
        }
        else {
            m_read_serial_count = 0;
            killTimer(m_timer_id);
            clear_station_info_and_post_to_mes();
        }
    }
}

void GDataFactory::clear_station_info_and_post_to_mes()
{
    //clear the lCode info of the 9 stations,clear the db lCode info of
    //the 9 stations
    QMutexLocker lock(&m_pDBMutex);
    QString strSql = QString("delete from public.%1 where "
                     "station_on_line   = '%2' or "
                     "station_silicon   = '%2' or "
                     "station_radiator  = '%2' or "
                     "station_brace     = '%2' or "
                     "station_pcba      = '%2' or "
                     "station_diode     = '%2' or "
                     "station_igbt      = '%2' or "
                     "station_out_plate = '%2' or "
                     "station_line_order= '%2'").
            arg(constStationInfoTable).arg(m_pCurrentLabelCode);

    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        //del from the class obj
        FinalProductInfo finalInfo;
        finalInfo.init_value();
        finalInfo.label_code = m_pCurrentLabelCode;
        for(int i =0;i<m_pStationList.size();i++)
        {
            if(m_pStationList[i]->del_label_code(m_pCurrentLabelCode))
            {
                switch (m_pStationList[i]->get_station_type()) {
                case ONLINE:
                    finalInfo.on_line_pass = true;
                    break;
                case SILICON:
                    finalInfo.silicon_vision_info.silicon_pass = true;
//                    finalInfo.silicon_vision_info.silicon_vision_result = get_station_silicon()->get_vision_sign(lCode);
                    break;
                case RADIATOR:
                    finalInfo.radiator_pass = true;
                    break;
                default:
                    break;
                }
                m_pStationList[i]->update_station_info_after_del(m_pStationList[i]);
            }
        }
        m_pCurrentQrCode = "";
    }
    else
    {
        QLOG_WARN()<<"del from db failed:"<<strError;
        m_pCurrentQrCode = "";
    }
}

void GDataFactory::slot_notify_update_db(STATIONTYPE sType, QString lCode)
{
    QMutexLocker lock(&m_pDBMutex);
    int currentID = get_biggest_id(constStationInfoTable);
    QString strSql="";
    if(sType == RADIATOR)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg(lCode).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("");
    }
    else if(sType == BRACE)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg("").
                    arg(lCode).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("");
    }
    else if(sType == DIODE)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg(lCode).
                    arg("").
                    arg("").
                    arg("");
    }
    else if(sType == IGBT)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg(lCode).
                    arg("").
                    arg("");
    }
    else if(sType == LINEORDER)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg(lCode);
    }
    else if(sType == ONLINE)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg(lCode).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("");

    }
    else if(sType == OUTPLATE)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg(lCode).
                    arg("");
    }
    else if(sType == PCBA)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg(lCode).
                    arg("").
                    arg("").
                    arg("").
                    arg("");
    }
    else if(sType == SILICON)
    {
        strSql = QString("insert into public.%1 values(%2,'%3','%4','%5','%6','%7','%8','%9','%10','%11')").
                    arg(constStationInfoTable).
                    arg(currentID).
                    arg("").
                    arg(lCode).
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("").
                    arg("");
    }
    else
        ;

    if(strSql == "")
    {
        QLOG_WARN()<<"strSql is NULL";
        return;
    }

    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        QLOG_INFO()<<"insert into station info table success!";
    }
    else
    {
        QLOG_WARN()<<"insert into station info table failed:"<<strError;
    }
}

void GDataFactory::slot_notify_update_paste_label_db(QString lCode)
{
    m_pCurrentLabelCode = lCode;
    QDateTime current_date_time = QDateTime::currentDateTime();
    QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
    QString strSql = QString("insert into %1 values('%2','%3')").
                arg(constStationPasteLabelInfoTable).
                arg(lCode).
                arg(current_date);
    QString strError;
    if(GDataFactory::get_pgsql()->ExecSql(strSql,strError))
    {
        ;
    }
    else
    {
        QLOG_WARN()<<"insert into paste label db failed:"<<strError;
    }

    m_timer_id = startTimer(500);

}

void GDataFactory::slot_rev_serial_number(const QByteArray &data)
{
    QLOG_INFO()<<"the response of serial port is:"<<data;
    QByteArray tmpData;
    for (int var = 6; var < data.size(); ++var) {
        tmpData.append(data[var]);
    }
    m_pCurrentQrCode = tmpData;
    QLOG_INFO()<<"current serial number is:"<<m_pCurrentQrCode.remove('\r');
    if(m_pCurrentQrCode != "")
    {
        QByteArray tmpArray;
        for(int i=0;i<2;i++)
            tmpArray.append(static_cast<char>(0x00));
        get_udp_service()->send_message_to_plc(WRITE_PLC,ADDRESS_D1019,2,tmpArray);
    }
    else {
        ;
    }
}

quint8 GDataFactory::get_last_ip(const int id)
{
    if (id == 0)
    {
        QString localHostName = QHostInfo::localHostName();
        QString localIP = "";
        QHostInfo info = QHostInfo::fromName(localHostName);
        foreach(QHostAddress addr,info.addresses())
            if(addr.protocol() == QAbstractSocket::IPv4Protocol)
            {
                localIP = addr.toString();
            }
        return process_ip_string(localIP);
    }
    else
    {
        return process_ip_string(get_config_para("PLC_IP"));
    }
}

quint8 GDataFactory::process_ip_string(const QString ipstr)
{
    int nSize = ipstr.size();
    int nCount = 0;
    QString mData;
    mData.clear();
    QChar mChar;
    for(int idx = 0; idx<nSize; idx++)
    {
        mChar = ipstr[idx];
        if(mChar == '.')
        {
            nCount++;
        }
        else if(mChar != '.' && nCount == 3 && (mChar >= '0' || mChar <= '9'))
        {
            mData.append(mChar);
        }
    }
    return static_cast<quint8>(mData.toInt());
}
