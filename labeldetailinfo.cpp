#include "labeldetailinfo.h"
#include <QVBoxLayout>
#include "TitleBar.h"
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QPainter>
#include "gdatafactory.h"
LabelDetailInfo::LabelDetailInfo(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
//    this->setAttribute(Qt::WA_TranslucentBackground);
//    this->setStyleSheet("QWidget{background-color:#000000}");
    resize(800,600);

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"标签详情");
    pTitleBar->setFixedWidth(800);
    installEventFilter(pTitleBar);

    grpBox = new QGroupBox();
    QVBoxLayout* vAll = new QVBoxLayout();
    QHBoxLayout* hBox1 = new QHBoxLayout();
    hBox1->addWidget(createTableWidget());
    grpBox->setLayout(hBox1);
    vAll->addSpacing(pTitleBar->height());
    vAll->addWidget(grpBox);
    this->setLayout(vAll);

}

QWidget *LabelDetailInfo::createTableWidget()
{
    QFont* font = new QFont("Microsoft YaHei", 20, QFont::StyleNormal);
    tableWidget = new QTableWidget(1000,1);
//    tableWidget->setEnabled(false);
    tableWidget->setAttribute(Qt::WA_TranslucentBackground);
    tableWidget->setGridStyle(Qt::PenStyle::SolidLine);
    tableWidget->setFont(*font);
    tableWidget->setWindowTitle("QTableWidget & Item");
    QStringList header;
    header<<u8"标签内容";
    tableWidget->setHorizontalHeaderLabels(header);
    tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{border-color:rgba(255,255,255,100);"
                                                   "color:rgb(255,255,255);"
                                                   "background-color:rgba(96,163,213,50);"
                                                   "font-family:'Microsoft YaHei';"
                                                   "font-size:20px;"
                                                   "font-weight:bold;}");
//    tableWidget->setStyleSheet("QTableWidget{color:rgb(0,0,0);"
//                               "background-color:rgba(0,0,0,0);}"
//                               "QTableWidget::item::selected{color:rgb(0,0,0);"
//                               "background-color:rgb(207,207,217);}"
//                               "QTableWidget::item{color:rgb(255,255,255)}");
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

//    tableWidget->verticalHeader()->setVisible(false);
    tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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

void LabelDetailInfo::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(this->rect(),QBrush(QColor(47,74,92)));
//    painter.fillRect(this->rect(),QBrush(QColor(96,163,213,50)));
}

void LabelDetailInfo::set_table_data()
{
    this->tableWidget->clearContents();
    QList<QString> lstName;

    if(this->grpBox->title() == u8"线头")
        lstName = GDataFactory::get_station_on_line()->get_label_code_list();
    else if(this->grpBox->title() ==  u8"硅脂视觉检")
        lstName = GDataFactory::get_station_silicon()->get_label_code_list();
    else if(this->grpBox->title() == u8"散热器")
        lstName = GDataFactory::get_station_radiator()->get_label_code_list();
    else if(this->grpBox->title() == u8"支撑架")
        lstName = GDataFactory::get_station_brace()->get_label_code_list();
    else if(this->grpBox->title() == u8"PCBA上线")
        lstName = GDataFactory::get_station_PCBA()->get_label_code_list();
    else if(this->grpBox->title() == u8"二极管")
        lstName = GDataFactory::get_station_diode()->get_label_code_list();
    else if(this->grpBox->title() == u8"IGBT")
        lstName = GDataFactory::get_station_IGBT()->get_label_code_list();
    else if(this->grpBox->title() == u8"外机板")
        lstName = GDataFactory::get_station_out_plate()->get_label_code_list();
    else if(this->grpBox->title() == u8"线序视觉检")
        lstName = GDataFactory::get_station_line_order()->get_label_code_list();
    else if(this->grpBox->title() == u8"贴标")
        lstName = GDataFactory::get_station_paste_label()->get_label_code_list();
    else {
        ;
    }

    for (int i=0;i<lstName.size();i++)
    {
        QTableWidgetItem *item = new QTableWidgetItem(lstName[i]);
        tableWidget->setItem(i,0,item);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));
        tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    }
}

void LabelDetailInfo::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    set_table_data();
}
