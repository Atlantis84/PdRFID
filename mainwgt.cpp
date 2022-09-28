#include "mainwgt.h"
#include "TitleBar.h"
#include <QVBoxLayout>
#include "gdatafactory.h"
#include <QDesktopWidget>
#include <QApplication>
#include <QPainter>
MainWgt::MainWgt(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags() | Qt::Widget);
    setWindowIcon(QIcon(":/icon/24x24.png"));
    this->setStyleSheet("QWidget{background-color:#000000}");
    resize(1600,800);
    QDesktopWidget* desktop = QApplication::desktop();
    int w,h;
    w = desktop->screenGeometry().width();
    h = desktop->screenGeometry().height();
    this->setGeometry(w/2-800,h/2-520,1600,800);

    TitleBar *pTitleBar = new TitleBar(this);
    pTitleBar->setTitle(u8"PD_ELEC_EQUIP_RFID");
    pTitleBar->setFixedWidth(1600);
    installEventFilter(pTitleBar);

    QVBoxLayout* vAll = new QVBoxLayout();
    vAll->addSpacing(pTitleBar->height());
    vAll->setSpacing(0);
    vAll->setMargin(0);
    GDataFactory::get_main_window()->setParent(this);
    vAll->addWidget(GDataFactory::get_main_window());
    this->setLayout(vAll);
}

void MainWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
//    painter.fillRect(this->rect(),QBrush(QColor(240,242,245)));
    painter.drawPixmap(this->rect(),QPixmap(":/icon/bk.png"));
    painter.fillRect(this->rect(),QBrush(QColor(96,163,213,100)));
}
