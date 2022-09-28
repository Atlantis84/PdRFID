#ifndef MAINWGT_H
#define MAINWGT_H

#include <QWidget>

class MainWgt : public QWidget
{
    Q_OBJECT
public:
    explicit MainWgt(QWidget *parent = nullptr);

signals:

public slots:

private:
    void paintEvent(QPaintEvent *event);
};

#endif // MAINWGT_H
