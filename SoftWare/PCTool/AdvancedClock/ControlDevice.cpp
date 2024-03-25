#include <QMessageBox>
#include "ControlDevice.h"
#include "easylogging++.h"
#include "Cmd.h"
#include "ui_ControlDevice.h"

ControlDevice::ControlDevice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlDevice)
{
    MyTcpServer = new QTcpServer;

    InternetPort = 53400;
    ConnectIP = nullptr;
    currentClient = nullptr;

    InitUI();
    InitConnect();
    ScanInternet();
    ui->setupUi(this);
}

void ControlDevice::InitUI()
{

}


void ControlDevice::InitConnect()
{
    connect(MyTcpServer,SIGNAL(newConnection()),this,SLOT(NewConnect()));
}

void ControlDevice::ScanInternet()
{
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
    if(addressList.empty())
    {
        return;
    }

    QList<QHostAddress> ipv4AddressList;
    foreach (QHostAddress address, addressList)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ipv4AddressList.append(address);
            QString temp_str = address.toString();
            LOG(INFO) << "find IP:" << temp_str.toStdString() << Qt::endl;

            /* find the match ip */
            int lastDot = temp_str.lastIndexOf('.') + 1;
            int IPLastNum = temp_str.mid(lastDot,temp_str.length()).toInt();
            /*LOG(INFO) << IPLastNum <<endl;*/

            /* the last num is 1, the ip is  gateway */
            if(1 != IPLastNum)
            {
                ConnectIP = temp_str;
                LOG(INFO) << "Connect IP is:"<< ConnectIP.toStdString() << Qt::endl;
            }
        }
    }

    if(ConnectIP == nullptr)
    {
        return;
    }

    bool is_ok = MyTcpServer->listen(QHostAddress(ConnectIP),InternetPort);
    if(is_ok)
    {

    }
}

void ControlDevice::CloseInternetConnect()
{
    MyTcpServer->disconnect();
    MyTcpServer->close();

    if(currentClient != NULL)
    {
        currentClient->disconnect();
        currentClient->close();
    }

    QMessageBox::information(NULL, "info", "close listen", QMessageBox::Yes, QMessageBox::NoButton);
}

void ControlDevice::NewConnect()
{
    currentClient = MyTcpServer->nextPendingConnection();
    MyTcpClient.append(currentClient);

    connect(currentClient, SIGNAL(readyRead()), this, SLOT(ReadData()));
    connect(currentClient, SIGNAL(disconnected()), this, SLOT(disconnectedSlot()));

    QString new_connect_info = "new connect,";
    new_connect_info += "ip:";
    new_connect_info += currentClient->peerAddress().toString();
    new_connect_info += ",port:";
    new_connect_info += QString::number(currentClient->peerPort());

    LOG(INFO) << new_connect_info.toStdString() << Qt::endl;
}

void ControlDevice::ReadData()
{
    QString disp_string,S;
    QByteArray buffer = currentClient->readAll();
    LOG(INFO) <<"len=" <<buffer.size()<<Qt::endl;;

    for(int i=0;i<buffer.size();i++)
    {
        S.sprintf("0x%02x, ", (unsigned char)buffer.at(i));
        disp_string += S;
    }

    LOG(INFO) << disp_string.toStdString()<<Qt::endl;
    RspDataProcess(buffer);
}

void ControlDevice::disconnectedSlot()
{
    for(int i=0; i<MyTcpClient.length(); i++)
    {
        if(MyTcpClient[i]->state() == QAbstractSocket::UnconnectedState)
        {
             MyTcpClient[i]->destroyed();
             MyTcpClient.removeAt(i);
        }
    }
}

void ControlDevice::GetUSBInfo(uint8_t data_type)
{
    uint8_t buf[sizeof(cmd_msg_frame_t) + sizeof(uint8_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint8_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = GET_USB_INFO;
    msg->seq = 0x00;
    msg->data_len = sizeof(uint8_t);

    uint8_t *set_data = (uint8_t *)(msg + 1);
    *set_data = data_type;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(uint8_t));
    uint32_t *check_sum = (uint32_t *)(set_data+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
}

static void MessageBoxShow(QString str)
{
    QMessageBox::warning(NULL, "warning", str, QMessageBox::Yes, QMessageBox::NoButton);
}
void  ControlDevice::RspDataProcess(QByteArray buf)
{
    uint8_t *data = (uint8_t *)buf.data();
    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)data;
    char format_data[64];
    if(MSG_FRAME_HEADER != msg->header)
    {
        sprintf(format_data,"header err=%d",msg->header);
        QString date_str(format_data);
        MessageBoxShow(date_str);
        return;
    }

    switch(msg->cmd)
    {
        case GET_USB_INFO:
        {

        }
        break;
        default:
        {

        }
    }
}

ControlDevice::~ControlDevice()
{
    delete ui;
}
