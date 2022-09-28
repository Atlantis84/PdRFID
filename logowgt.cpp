#include "logowgt.h"
#include <QPainter>
#include <QVBoxLayout>
LogoWgt::LogoWgt(QWidget *parent) : QWidget(parent)
{

    QVBoxLayout* vAll = new QVBoxLayout();
    this->setLayout(vAll);
}

void LogoWgt::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QPixmap tmpPixmap = QPixmap(":/icon/2.png");
    int l = this->width()/4;
    int t = 0;
    int w = this->width()/2;
    int h = this->height();
    painter.drawPixmap(l,t,w,h,tmpPixmap);
}
