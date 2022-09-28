#ifndef LABELDETAILINFO_H
#define LABELDETAILINFO_H

#include <QWidget>
#include <QGroupBox>
#include <QTableWidget>
class LabelDetailInfo : public QWidget
{
    Q_OBJECT
public:
    explicit LabelDetailInfo(QWidget *parent = nullptr);
    void set_text(QString strName)
    {
        this->grpBox->setTitle(strName);
    }
    void set_table_data();

private:
    QGroupBox* grpBox;
    QTableWidget* tableWidget;
    QWidget* createTableWidget();
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);

signals:

public slots:
};

#endif // LABELDETAILINFO_H
