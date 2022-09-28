#ifndef STATIONBASE_H
#define STATIONBASE_H

#include <QObject>
#include <QMap>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include "QsLog.h"
enum STATIONTYPE{
    RADIATOR = 0,       //散热器
    BRACE = 1,          //支撑架
    DIODE = 2,          //二极管
    IGBT = 3,           //IGBT
    LINEORDER = 4,      //线序检
    ONLINE = 5,         //产品上线
    OUTPLATE = 6,       //外机板
    PASTELABEL = 7,     //贴标
    PCBA = 8,           //PCBA
    SILICON = 9,         //硅脂
    BASE = 10
};

struct SiliconVisionInfo
{
    bool silicon_pass;
    bool silicon_vision_result;//true presents OK; false presents NG
};
struct LineOrderVisionInfo
{
    bool line_order_pass;
    bool line_order_vision_result;//true presents OK; false presents NG
};

struct FinalProductInfo
{
    QString label_code;
    QString Qr_code;
    bool on_line_pass;
    SiliconVisionInfo silicon_vision_info;
    bool radiator_pass;
    bool brace_pass;
    bool PCBA_pass;
    bool diode_pass;
    bool IGBT_pass;
    bool out_plate_pass;
    LineOrderVisionInfo line_order_vision_info;
    void init_value(){
        label_code = "";
        Qr_code = "";
        on_line_pass = false;
        silicon_vision_info.silicon_pass = false;
        silicon_vision_info.silicon_vision_result = false;
        radiator_pass = false;
        brace_pass = false;
        PCBA_pass = false;
        diode_pass = false;
        IGBT_pass = false;
        out_plate_pass = false;
        line_order_vision_info.line_order_pass = false;
        line_order_vision_info.line_order_vision_result = false;
    }
};

class StationBase : public QObject
{
    Q_OBJECT
public:
    explicit StationBase(QObject *parent = nullptr);

    void add_label_code(QString lCode)
    {
        if(this->m_pLabelCode.contains(lCode))
            QLOG_INFO()<<"the current code:"<<lCode<<" exists already";
        else
        {
            this->m_pLabelCode.push_back(lCode);
            emit signal_notify_station_info((StationBase*)this);
            emit signal_notify_update_db(this->m_pProperty,lCode);
        }
    }

    void pre_add_label_code(QString lCode)
    {
        if(this->m_pLabelCode.contains(lCode))
            QLOG_INFO()<<"the current code:"<<lCode<<" exists already";
        else
        {
            this->m_pLabelCode.push_back(lCode);
        }
    }

    bool del_label_code(QString lCode)
    {
        return this->m_pLabelCode.removeOne(lCode);
    }

    QList<QString> get_label_code_list()
    {
        return this->m_pLabelCode;
    }

    STATIONTYPE get_station_type()
    {
        return m_pProperty;
    }

    void set_station_type(STATIONTYPE type)
    {
        m_pProperty = type;
    }

    void update_station_info_after_del(StationBase* sb)
    {
        emit signal_notify_station_info(sb);
    }

public:
    QList<QString> m_pLabelCode;
protected:
    QMutex m_mutex;
    STATIONTYPE m_pProperty;

signals:
    void signal_notify_station_info(StationBase*);
    void signal_notify_update_db(STATIONTYPE,QString);
    void signal_notify_update_paste_label_db(QString);

public slots:
};

#endif // STATIONBASE_H
