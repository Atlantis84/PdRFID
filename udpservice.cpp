#include "udpservice.h"
#include "gdatafactory.h"
#include "QsLog.h"
UdpService::UdpService(QObject *parent) : QObject(parent)
{
    m_pUdpSocket = new QUdpSocket(this);
    m_pUdpSocket->bind(QHostAddress("192.168.0.188"),59999);
    connect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    connect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
}

void UdpService::release_res()
{
    disconnect(m_pUdpSocket,SIGNAL(readyRead()),this,SLOT(slot_receive_message()));
    disconnect(this,SIGNAL(signal_proc_plc_msg(const QByteArray)),this,SLOT(slot_proc_plc_msg(const QByteArray)));
    if(m_pUdpSocket != nullptr)
        delete m_pUdpSocket;
    m_pUdpSocket = nullptr;
}

void UdpService::send_message_to_plc(const uchar function_code, const short data_address, const short data_length, const QByteArray data_array)
{
    m_mutex.lock();
    QHostAddress hAddr(GDataFactory::get_factory()->get_config_para("PLC_IP"));
    int hPort = GDataFactory::get_factory()->get_config_para("PLC_PORT").toInt();
    QByteArray btArray = make_plc_message(function_code,data_address,data_length,data_array);
//    QLOG_INFO()<<"Send Msg To PLC:"<<GDataFactory::get_factory()->bytes_to_str(btArray);
    m_pUdpSocket->writeDatagram(btArray,btArray.length(),hAddr,static_cast<quint16>(hPort));
    m_mutex.unlock();
}

QByteArray UdpService::make_plc_message(const uchar function_code, const short data_address, const short data_length, const QByteArray data_array)
{
    QByteArray m_data;
    m_data.append(static_cast<char>(0x80));  //ÆðÊ¼Âë
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x02));
    m_data.append(static_cast<char>(0x00));  //PLC IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(1)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));  //self IP Address
    m_data.append(static_cast<char>(GDataFactory::get_factory()->get_last_ip(0)));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0xFF));  //SID
    m_data.append(static_cast<char>(0x01));  //FINS

    switch (function_code)
    {
    case 1://read plc
        m_data.append(static_cast<char>(0x01));
        break;
    case 2://write plc
        m_data.append(static_cast<char>(0x02));
        break;
    }

    m_data.append(static_cast<char>(0x82));
    m_data.append(static_cast<char>(0xFF&(data_address>>8)));
    m_data.append(static_cast<char>(0xFF&data_address));
    m_data.append(static_cast<char>(0x00));
    int length = data_length/2;//plc recognize by word
    m_data.append(static_cast<char>(0xFF&(length>>8)));
    m_data.append(static_cast<char>(0xFF&length));

    if(function_code == 2)
        m_data.append(data_array);

    return m_data;
}

void UdpService::slot_receive_message()
{
    m_mutex.lock();
    if(m_pUdpSocket == nullptr)
        return;
    QByteArray datagram;
    datagram.resize(static_cast<int>(m_pUdpSocket->pendingDatagramSize()));
    while(m_pUdpSocket->hasPendingDatagrams())
    {
        m_pUdpSocket->readDatagram(datagram.data(), datagram.size());
    }
    m_mutex.unlock();
    emit signal_proc_plc_msg(datagram);
}

void UdpService::slot_proc_plc_msg(const QByteArray data_array)
{
//    QLOG_INFO()<<"REV PLC MSG:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
    if(data_array.length() > 10)//check the header length
    {
        if((data_array[10] == 0x01) && (data_array[11] == 0x01))
        {
            if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
            {
//                QLOG_INFO()<<"Rev Read Back Msg From PLC:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
                QByteArray tmpArray;
                for(int i=14;i<data_array.length();i++)
                    tmpArray.append(data_array[i]);
                emit signal_proc_station_info_from_plc(tmpArray);
            }
            else
            {
                QLOG_WARN()<<"PLC read back message is ABNORMAL!";
            }
        }
        else if((data_array[10] == 0x01) && (data_array[11] == 0x02))
        {
            if((data_array[12] == 0x00)&&(data_array[13] == 0x00))
            {
                ;
//                ;QLOG_INFO()<<"Rev Write Back Msg From PLC:"<<GDataFactory::get_factory()->bytes_to_str(data_array);
            }
            else
            {
                QLOG_WARN()<<"PLC write back message ABNORMAL!";
            }
        }
    }
    else
    {
        QLOG_WARN()<<"PLC message header lenght is WRONG!";
    }

}
