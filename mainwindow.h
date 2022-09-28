#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QMenu *viewMenu;

    void createActions();
    void createStatusBar();
    void upload_file(QString filewithpath);
    void download_file(QString filename);
    void paintEvent(QPaintEvent *event);
    void timerEvent(QTimerEvent *event);
    int m_pTimerId;
    int m_pMonitorTimerId;
    int m_pMonitorCount;
public slots:
    void slot_calibration();
    void slot_start_run();
    void slot_proc_station_info_from_plc(const QByteArray data);
};

#endif // MAINWINDOW_H
