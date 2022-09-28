#ifndef REALTIMELOGWGT_H
#define REALTIMELOGWGT_H

#include <QWidget>
#include <QTextEdit>
class RealTimeLogWgt : public QWidget
{
    Q_OBJECT
public:
    explicit RealTimeLogWgt(QWidget *parent = nullptr);
private:
    void paintEvent(QPaintEvent *event);
    QTextEdit *m_pLogEdit;
signals:

public slots:
    void slot_rev_logs(const QString strMsg);
};

#endif // REALTIMELOGWGT_H
