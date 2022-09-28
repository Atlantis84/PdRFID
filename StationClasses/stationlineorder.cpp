#include "stationlineorder.h"

StationLineOrder::StationLineOrder()
{
    this->set_station_type(STATIONTYPE::LINEORDER);
    m_pVisionDetectSign = false;
}
