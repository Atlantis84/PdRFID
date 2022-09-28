#include "tcpsocket.h"
#include "QsLog.h"
#include "gdatafactory.h"
TcpSocket::TcpSocket(QObject *parent) : QThread(parent)
{
    m_pTcpSocketFront = nullptr;
    m_pTcpFrontConnectSign = false;
    m_pTcpSocketMiddle = nullptr;
    m_pTcpMiddleConnectSign = false;
    m_pTcpSocketBack = nullptr;
    m_pTcpBackConnectSign = false;
}

bool TcpSocket::connect_to_front_server(QString frontIp, int frontPort)
{
    m_pTcpSocketFront->connectToHost(frontIp,frontPort);
    if(m_pTcpSocketFront->waitForConnected(1000))
    {
        m_pTcpFrontConnectSign = true;
        connect(m_pTcpSocketFront,SIGNAL(readyRead()),this,SLOT(slot_rev_from_front_server()),Qt::DirectConnection);
        return true;
    }
    else {
        return false;
    }
}

void TcpSocket::send_to_front_server(QByteArray data)
{
    if(m_pTcpFrontConnectSign)
    {
        m_front_mutex.lock();
        m_pFrontQueue.enqueue(data);
        m_front_mutex.unlock();
    }
    else
        QLOG_INFO()<<"the front server is not connected";
}

bool TcpSocket::connect_to_middle_server(QString middleIp, int middlePort)
{
    m_pTcpSocketMiddle->connectToHost(middleIp,middlePort);
    if(m_pTcpSocketMiddle->waitForConnected(1000))
    {
        m_pTcpMiddleConnectSign = true;
        connect(m_pTcpSocketMiddle,SIGNAL(readyRead()),this,SLOT(slot_rev_from_middle_server()));
        return true;
    }
    else {
        return false;
    }

}

void TcpSocket::send_to_middle_server(QByteArray data)
{
//    QMutexLocker lock(&m_mutex);
    if(m_pTcpMiddleConnectSign)
    {
        m_middle_mutex.lock();
        m_pMiddleQueue.enqueue(data);
        m_middle_mutex.unlock();
    }
    else
        QLOG_INFO()<<"the middle server is not connected";
}

bool TcpSocket::connect_to_back_server(QString backIp, int backPort)
{
    m_pTcpSocketBack->connectToHost(backIp,backPort);
    if(m_pTcpSocketBack->waitForConnected(1000))
    {
        m_pTcpBackConnectSign = true;
        connect(m_pTcpSocketBack,SIGNAL(readyRead()),this,SLOT(slot_rev_from_back_server()));
        return true;
    }
    else {
        return false;
    }
}

void TcpSocket::send_to_back_server(QByteArray data)
{
//    QMutexLocker lock(&m_mutex);
    if(m_pTcpBackConnectSign)
    {
        m_back_mutex.lock();
        m_pBackQueue.enqueue(data);
        m_back_mutex.unlock();
    }
    else
        QLOG_INFO()<<"the back server is not connected";
}

void TcpSocket::run()
{
    while (1) {
        if(m_pTcpSocketFront == nullptr)
        {
            m_pTcpSocketFront = new QTcpSocket();
            connect_to_front_server(
                        GDataFactory::get_factory()->get_config_para("FRONT_RWRITE_IP"),
                        GDataFactory::get_factory()->get_config_para("FRONT_RWRITE_PORT").toInt());
        }

        if(m_pTcpSocketMiddle == nullptr)
        {
            m_pTcpSocketMiddle = new QTcpSocket();
            connect_to_middle_server(
                        GDataFactory::get_factory()->get_config_para("MIDDLE_RWRITE_IP"),
                        GDataFactory::get_factory()->get_config_para("MIDDLE_RWRITE_PORT").toInt());
        }

        if(m_pTcpSocketBack == nullptr)
        {
            m_pTcpSocketBack = new QTcpSocket();
            connect_to_back_server(
                        GDataFactory::get_factory()->get_config_para("BACK_RWRITE_IP"),
                        GDataFactory::get_factory()->get_config_para("BACK_RWRITE_PORT").toInt());
        }

        while(!m_pFrontQueue.isEmpty())
        {
            m_front_mutex.lock();
            const QByteArray data = m_pFrontQueue.dequeue();
            m_front_mutex.unlock();
            m_pTcpSocketFront->write(data,data.length());
            m_pTcpSocketFront->flush();
            m_pTcpSocketFront->waitForReadyRead();
        }
        while(!m_pMiddleQueue.isEmpty())
        {
            m_middle_mutex.lock();
            const QByteArray data = m_pMiddleQueue.dequeue();
            m_middle_mutex.unlock();
            m_pTcpSocketMiddle->write(data);
            m_pTcpSocketMiddle->flush();
            m_pTcpSocketMiddle->waitForReadyRead();
        }
        while(!m_pBackQueue.isEmpty())
        {
            m_back_mutex.lock();
            const QByteArray data = m_pBackQueue.dequeue();
            m_pTcpSocketBack->write(data);
            m_pTcpSocketBack->flush();
            m_pTcpSocketBack->waitForReadyRead();
            m_back_mutex.unlock();
        }
        QThread::msleep(200);
    }
}

void TcpSocket::slot_rev_from_front_server()
{
    QString strMsg = m_pTcpSocketFront->readAll();
    m_pTcpSocketFront->flush();
    QLOG_INFO()<<"rev from front-server:"<<strMsg;
    if(strMsg.mid(0,1) == "0")
        GDataFactory::get_station_on_line()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "1")
        GDataFactory::get_station_silicon()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "2")
        GDataFactory::get_station_radiator()->add_label_code(strMsg.mid(2,3));
    else
        ;
}

void TcpSocket::slot_rev_from_middle_server()
{
    QString strMsg = m_pTcpSocketMiddle->readAll();
    m_pTcpSocketMiddle->flush();
    QLOG_INFO()<<"rev from middle-server:"<<strMsg;
    if(strMsg.mid(0,1) == "3")
        GDataFactory::get_station_brace()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "4")
        GDataFactory::get_station_PCBA()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "5")
        GDataFactory::get_station_diode()->add_label_code(strMsg.mid(2,3));
    else
        ;
}

void TcpSocket::slot_rev_from_back_server()
{
    QString strMsg = m_pTcpSocketBack->readAll();
    m_pTcpSocketBack->flush();
    QLOG_INFO()<<"rev from back-server:"<<strMsg;
    if(strMsg.mid(0,1) == "6")
        GDataFactory::get_station_IGBT()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "7")
        GDataFactory::get_station_out_plate()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "8")
        GDataFactory::get_station_line_order()->add_label_code(strMsg.mid(2,3));
    else if(strMsg.mid(0,1) == "9")
        GDataFactory::get_station_paste_label()->add_label_code(strMsg.mid(2,3));
    else
        ;
}
