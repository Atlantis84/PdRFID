#include "stationbase.h"

StationBase::StationBase(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<STATIONTYPE>("STATIONTYPE");
}
