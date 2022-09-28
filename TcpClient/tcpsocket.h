#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <QQueue>
#include <QMutex>
#include <QMutexLocker>
class TcpSocket : public QThread
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = nullptr);

    bool connect_to_front_server(QString frontIp,int frontPort);
    void send_to_front_server(QByteArray data);
    bool connect_to_middle_server(QString middleIp,int middlePort);
    void send_to_middle_server(QByteArray data);
    bool connect_to_back_server(QString backIp,int backPort);
    void send_to_back_server(QByteArray data);
    void run();
private:
    QTcpSocket* m_pTcpSocketFront;
    bool m_pTcpFrontConnectSign;
    QTcpSocket* m_pTcpSocketMiddle;
    bool m_pTcpMiddleConnectSign;
    QTcpSocket* m_pTcpSocketBack;
    bool m_pTcpBackConnectSign;
    QQueue<QByteArray> m_pFrontQueue;
    QQueue<QByteArray> m_pMiddleQueue;
    QQueue<QByteArray> m_pBackQueue;

    QMutex m_mutex;
    QMutex m_front_mutex;
    QMutex m_middle_mutex;
    QMutex m_back_mutex;

signals:
public slots:
    void slot_rev_from_front_server();
    void slot_rev_from_middle_server();
    void slot_rev_from_back_server();
};

#endif // TCPSOCKET_H
