#ifndef STATIONSILICON_H
#define STATIONSILICON_H
#include "stationbase.h"
//¹èÖ¬ÊÓ¾õ¼ì-RFID²É¼¯
class StationSilicon : public StationBase
{
public:
    StationSilicon();

    void add_label_code(QString lCode)
    {
        if(this->m_pLabelCode.contains(lCode))
            QLOG_INFO()<<"the current code:"<<lCode<<" exists already";
        else
        {
            m_current_code = lCode;
            this->m_pLabelCode.push_back(lCode);
            this->m_vision_result.insert(lCode,false);
            emit signal_notify_station_info((StationBase*)this);
            emit signal_notify_update_db(this->m_pProperty,lCode);
        }
    }

    bool get_vision_sign(QString lCode)
    {
        return m_vision_result[lCode];
    }

    void set_vision_sign(bool vSign)
    {
        m_vision_result[m_current_code] = vSign;
    }
private:
    bool m_pVisionDetectSign;
    QMutex m_pVisionMutex;
    QMap<QString,bool> m_vision_result;
    QString m_current_code;
};

#endif // STATIONSILICON_H
