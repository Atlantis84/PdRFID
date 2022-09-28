#include "centralwgt.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include "gdatafactory.h"

CentralWgt::CentralWgt(QWidget *parent) : QWidget(parent)
{

    QHBoxLayout* hAll = new QHBoxLayout();
    QVBoxLayout* vBoxLeft = new QVBoxLayout();
    vBoxLeft->addWidget(GDataFactory::get_station_info_wgt());

    QVBoxLayout* vBoxRight = new QVBoxLayout();
    vBoxRight->addWidget(GDataFactory::get_detail_info_wgt());
    vBoxRight->addWidget(GDataFactory::get_real_time_log_wgt());
    vBoxRight->addWidget(GDataFactory::get_logo_wgt());
    vBoxRight->setStretch(0,5);
    vBoxRight->setStretch(1,7);
    vBoxRight->setStretch(2,2);

//    QGroupBox* groupBoxLeft = new QGroupBox(u8"Í¼ÏñA");
//    QGroupBox* groupBoxRight = new QGroupBox(u8"Í¼ÏñB");
//    groupBoxRight->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");
//    groupBoxLeft->setStyleSheet("QGroupBox{border:1px solid rgba(0,0,0,100);color:rgb(0,0,0);background-color:rgba(0,0,0,0);}");

//    groupBoxLeft->setLayout(vBoxLeft);
//    groupBoxRight->setLayout(vBoxRight);
//    hAll->addWidget(groupBoxLeft);
//    hAll->addWidget(groupBoxRight);
    hAll->addLayout(vBoxLeft);
    hAll->addLayout(vBoxRight);
    hAll->setStretch(0,1);
    hAll->setStretch(1,1);
    this->setLayout(hAll);
}
