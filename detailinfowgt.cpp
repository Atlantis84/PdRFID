#include "detailinfowgt.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPainter>
#include "gdatafactory.h"
DetailInfoWgt::DetailInfoWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addWidget(createTableWidget());

    vAll->addSpacing(50);
    vAll->addLayout(hBox1);
    this->setLayout(vAll);
}

QWidget *DetailInfoWgt::createTableWidget()
{
    QFont* font = new QFont("Microsoft YaHei", 20, QFont::StyleNormal);
    tableWidget = new QTableWidget(3,3);
    tableWidget->setEnabled(false);
    tableWidget->setAttribute(Qt::WA_TranslucentBackground);
    tableWidget->setGridStyle(Qt::PenStyle::SolidLine);
    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8" 读写器名称"<<u8"读写器地址"<<u8"读写器端口";
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
                               "background-color:rgb(207,207,217);}"
                               "QTableWidget::item{color:rgb(255,255,255)}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tableWidget->verticalHeader()->setStyleSheet("QHeaderView::section{border-color:rgba(255,255,255,100);"
                                                 "color:rgb(255,255,255,0);"
                                                 "width:0px;"
                                                 "background-color:rgba(216,217,222,0);"
                                                 "border-right-color:rgba(255,255,255,100);"
                                                 "border-top-color:rgba(0,0,0,0);"
                                                 "border-left-color:rgba(0,0,0,0);"
                                                 "border-bottom-color:rgba(0,0,0,0);}");
    tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    return tableWidget;
}

void DetailInfoWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(0,0,this->width(),50,QBrush(QColor(96,163,213,80)));
    painter.setFont(QFont(u8"Microsoft YaHei",16,QFont::Bold,false));
    painter.setPen(QPen(QColor(255,255,255,255)));
    painter.drawText(0,0,this->width(),50,Qt::AlignHCenter | Qt::AlignVCenter,u8"线体RFID读写器信息");
}

void DetailInfoWgt::showEvent(QShowEvent *event)
{
    set_table_data();
}

void DetailInfoWgt::set_table_data()
{
    this->tableWidget->clearContents();
    QList<QString> lstName;
    lstName<<u8"线体前读写器"<<u8"线体中读写器"<<u8"线体后读写器";
    QList<QString> lstIp,lstPort;
    lstIp<<GDataFactory::get_factory()->get_config_para("FRONT_RWRITE_IP")<<
           GDataFactory::get_factory()->get_config_para("MIDDLE_RWRITE_IP")<<
           GDataFactory::get_factory()->get_config_para("BACK_RWRITE_IP");
    lstPort<<GDataFactory::get_factory()->get_config_para("FRONT_RWRITE_PORT")<<
             GDataFactory::get_factory()->get_config_para("MIDDLE_RWRITE_PORT")<<
             GDataFactory::get_factory()->get_config_para("BACK_RWRITE_PORT");
    for (int i=0;i<3;i++)
    {
        tableWidget->setItem(i,0,new QTableWidgetItem(lstName[i]));
        tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->setItem(i,1,new QTableWidgetItem(lstIp[i]));
        tableWidget->item(i,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        tableWidget->setItem(i,2,new QTableWidgetItem(lstPort[i]));
        tableWidget->item(i,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

