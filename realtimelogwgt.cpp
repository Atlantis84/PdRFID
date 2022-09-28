#include "realtimelogwgt.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPainter>
RealTimeLogWgt::RealTimeLogWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    m_pLogEdit = new QTextEdit();
    m_pLogEdit->setReadOnly(true);
    m_pLogEdit->setStyleSheet("color:rgb(255,0,0);background-color:rgba(0,0,0,0);font: 12pt 'LiSu'");
    vAll->addSpacing(50);
    vAll->addWidget(m_pLogEdit);
    this->setLayout(vAll);
}

void RealTimeLogWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.fillRect(0,0,this->width(),50,QBrush(QColor(96,163,213,80)));
    painter.setFont(QFont(u8"Microsoft YaHei",16,QFont::Bold,false));
    painter.setPen(QPen(QColor(255,255,255,255)));
    painter.drawText(0,0,this->width(),50,Qt::AlignHCenter | Qt::AlignVCenter,u8"实时日志信息");
}

void RealTimeLogWgt::slot_rev_logs(const QString strMsg)
{
    this->m_pLogEdit->append(strMsg);
}
