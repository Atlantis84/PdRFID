#ifndef STATIONLINEORDER_H
#define STATIONLINEORDER_H
#include "stationbase.h"
//ÏßÐò¼ì-RFID²É¼¯
class StationLineOrder : public StationBase
{
public:
    StationLineOrder();
    bool get_vision_sign()
    {
        QMutexLocker lock(&m_pVisionMutex);
        return m_pVisionDetectSign;
    }

    void set_vision_sign(bool vSign)
    {
        QMutexLocker lock(&m_pVisionMutex);
        m_pVisionDetectSign = vSign;
    }
private:
    bool m_pVisionDetectSign;
    QMutex m_pVisionMutex;
};

#endif // STATIONLINEORDER_H
