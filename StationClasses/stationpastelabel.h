#ifndef STATIONPASTELABEL_H
#define STATIONPASTELABEL_H
#include "stationbase.h"
//Ìù±ê-RFID²É¼¯
class StationPasteLabel : public StationBase
{
    Q_OBJECT
public:
    StationPasteLabel();
    void post_final_product_info(FinalProductInfo productInfo);
    void add_label_code(QString lCode)
    {
        this->m_pLabelCode.push_back(lCode);
        emit signal_notify_station_info((StationBase*)this);
        emit signal_notify_update_paste_label_db(lCode);
    }

    void pre_add_label_code(QString lCode)
    {
        this->m_pLabelCode.push_back(lCode);
    }
signals:

private:
};

#endif // STATIONPASTELABEL_H
