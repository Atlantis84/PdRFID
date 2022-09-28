#include "stationinfowgt.h"
#include <QPainter>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QSignalMapper>
#include <QDebug>
#include "gdatafactory.h"
StationInfoWgt::StationInfoWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    vAll->addSpacing(50);
    vAll->addWidget(createTableWidget());
    this->setLayout(vAll);
}

void StationInfoWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(0,0,this->width(),50,QBrush(QColor(96,163,213,80)));
    painter.setFont(QFont(u8"Microsoft YaHei",16,QFont::Bold,false));
    painter.setPen(QPen(QColor(255,255,255,255)));
    painter.drawText(0,0,this->width(),50,Qt::AlignHCenter | Qt::AlignVCenter,u8"站点标签信息");
}

void StationInfoWgt::showEvent(QShowEvent *event)
{
    set_table_data();
}

QWidget *StationInfoWgt::createTableWidget()
{
    QFont* font = new QFont("Microsoft YaHei", 20, QFont::StyleNormal);
    tableWidget = new QTableWidget(10,3);
//    tableWidget->setEnabled(false);
    tableWidget->setAttribute(Qt::WA_TranslucentBackground);
    tableWidget->setGridStyle(Qt::PenStyle::SolidLine);
    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8" 站点名称"<<u8"站点标签数目"<<u8"标签详细信息";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgba(255,255,255,100);"
                                                   "color:rgb(255,255,255);"
                                                   "background-color:rgba(96,163,213,50);"
                                                   "font-family:'Microsoft YaHei';"
                                                   "font-size:20px;"
                                                   "font-weight:bold;}");
    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,255);}"
                               "QTableWidget::item::selected{color:rgb(0,0,0);"
                               "background-color:rgba(0,0,0,0);}"
                               "QTableWidget::item{color:rgb(255,255,255)}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgba(255,255,255,100);"
                                                 "width:0px;"
                                                 "color:rgb(255,255,255,0);"
                                                 "background-color:rgba(216,217,222,0);"
                                                 "border-right-color:rgba(255,255,255,100);"
                                                 "border-top-color:rgba(0,0,0,0);"
                                                 "border-left-color:rgba(0,0,0,0);"
                                                 "border-bottom-color:rgba(0,0,0,0);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    return tableWidget;
}

void StationInfoWgt::set_table_data()
{
    QList<QString> lstName;
    lstName<<u8"线头"<<u8"硅脂视觉检"<<u8"散热器"<<
             u8"支撑架"<<u8"PCBA上线"<<u8"二极管"<<u8"IGBT"<<
             u8"外机板"<<u8"线序视觉检"<<u8"贴标";
    QList<QString> lstIp;
    lstIp<<QString::number(GDataFactory::get_station_on_line()->get_label_code_list().size())
        <<QString::number(GDataFactory::get_station_silicon()->get_label_code_list().size())
       <<QString::number(GDataFactory::get_station_radiator()->get_label_code_list().size())
      <<QString::number(GDataFactory::get_station_brace()->get_label_code_list().size())
     <<QString::number(GDataFactory::get_station_PCBA()->get_label_code_list().size())
    <<QString::number(GDataFactory::get_station_diode()->get_label_code_list().size())
    <<QString::number(GDataFactory::get_station_IGBT()->get_label_code_list().size())
    <<QString::number(GDataFactory::get_station_out_plate()->get_label_code_list().size())
    <<QString::number(GDataFactory::get_station_line_order()->get_label_code_list().size())
    <<QString::number(GDataFactory::get_station_paste_label()->get_label_code_list().size());
    QPushButton* btnOnLine = new QPushButton(u8"标签信息");
    QPushButton* btnSilicon = new QPushButton(u8"标签信息");
    QPushButton* btnRadiator = new QPushButton(u8"标签信息");
    QPushButton* btnBrace = new QPushButton(u8"标签信息");
    QPushButton* btnPCBA = new QPushButton(u8"标签信息");
    QPushButton* btnDiode = new QPushButton(u8"标签信息");
    QPushButton* btnIGBT = new QPushButton(u8"标签信息");
    QPushButton* btnOutPlate = new QPushButton(u8"标签信息");
    QPushButton* btnLineOrder = new QPushButton(u8"标签信息");
    QPushButton* btnPasteLabel = new QPushButton(u8"标签信息");
    QList<QPushButton*> lstBtn;
    lstBtn<<btnOnLine<<btnSilicon<<btnRadiator<<btnBrace<<btnPCBA<<
            btnDiode<<btnIGBT<<btnOutPlate<<btnLineOrder<<btnPasteLabel;
    QSignalMapper* mapper = new QSignalMapper();
    for (int i=0;i<10;i++)
    {
        lstBtn[i]->setStyleSheet("QPushButton{background-color:rgba(0,0,0,0);} "
                                 "QPushButton:hover{border:1px solid;"
                                 "border-radius:5px;"
                                 "border-color:rgba(0,0,0,50);"
                                 "background-color:rgba(96,163,213,80);}");
        mapper->setMapping(lstBtn[i],i);
        connect(lstBtn[i],SIGNAL(clicked()),mapper,SLOT(map()));
        tableWidget->setItem(i,0,new QTableWidgetItem(lstName[i]));
        tableWidget->item(i,0)->setFlags(Qt::ItemIsEnabled);
        tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->setItem(i,1,new QTableWidgetItem(lstIp[i]));
        tableWidget->item(i,1)->setFlags(Qt::ItemIsEnabled);
        tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->setCellWidget(i,2,lstBtn[i]);
    }
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(slot_btn_click(int)));
}

void StationInfoWgt::slot_btn_click(int index)
{
    switch(index)
    {
    case 0:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"线头");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 1:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"硅脂视觉检");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 2:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"散热器");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 3:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"支撑架");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 4:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"PCBA上线");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 5:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"二极管");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 6:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"IGBT");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 7:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"外机板");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 8:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"线序视觉检");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    case 9:
        GDataFactory::get_label_detail_info_wgt()->set_text(u8"贴标");
        GDataFactory::get_label_detail_info_wgt()->show();
        break;
    }
}

void StationInfoWgt::slot_notify_station_info(StationBase* base)
{
    if(base->get_station_type() == ONLINE)
    {
        QLOG_INFO()<<"update online table info";
        tableWidget->item(0,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == BRACE)
    {
        QLOG_INFO()<<"update brace table info";
        tableWidget->item(3,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == DIODE)
    {
        QLOG_INFO()<<"update diode table info";
        tableWidget->item(5,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == IGBT)
    {
        QLOG_INFO()<<"update IGBT table info";
        tableWidget->item(6,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == LINEORDER)
    {
        QLOG_INFO()<<"update lineorder table info";
        tableWidget->item(8,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == OUTPLATE)
    {
        QLOG_INFO()<<"update outplate table info";
        tableWidget->item(7,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == PASTELABEL)
    {
        QLOG_INFO()<<"update pastelabel table info";
        QLOG_INFO()<<QString::number(base->get_label_code_list().size());
        tableWidget->item(9,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == PCBA)
    {
        QLOG_INFO()<<"update PCBA table info";
        tableWidget->item(4,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == RADIATOR)
    {
        QLOG_INFO()<<"update radiator table info";
        tableWidget->item(2,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else if(base->get_station_type() == SILICON)
    {
        QLOG_INFO()<<"update silicon table info";
        tableWidget->item(1,1)->setText(QString::number(base->get_label_code_list().size()));
    }
    else {
        ;
    }
}
