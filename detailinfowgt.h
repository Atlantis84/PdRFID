#ifndef DETAILINFOWGT_H
#define DETAILINFOWGT_H

#include <QWidget>
class QTableWidget;
class DetailInfoWgt : public QWidget
{
    Q_OBJECT
public:
    explicit DetailInfoWgt(QWidget *parent = nullptr);

signals:

public slots:
private:
    QWidget* createTableWidget();
    QTableWidget* tableWidget;
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void set_table_data();
};

#endif // DETAILINFOWGT_H
