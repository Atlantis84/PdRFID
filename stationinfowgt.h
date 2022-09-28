#ifndef STATIONINFOWGT_H
#define STATIONINFOWGT_H

#include <QWidget>
#include <QTableWidget>
#include "stationbase.h"
class StationInfoWgt : public QWidget
{
    Q_OBJECT
public:
    explicit StationInfoWgt(QWidget *parent = nullptr);

    QWidget* createTableWidget();

private:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    QTableWidget* tableWidget;
    void set_table_data();
signals:

public slots:
    void slot_btn_click(int index);
    void slot_notify_station_info(StationBase* base);
};

#endif // STATIONINFOWGT_H
