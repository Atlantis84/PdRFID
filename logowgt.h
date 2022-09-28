#ifndef LOGOWGT_H
#define LOGOWGT_H

#include <QWidget>

class LogoWgt : public QWidget
{
    Q_OBJECT
public:
    explicit LogoWgt(QWidget *parent = nullptr);

private:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
};

#endif // LOGOWGT_H
