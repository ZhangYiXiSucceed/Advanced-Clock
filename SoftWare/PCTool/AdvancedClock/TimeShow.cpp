#include "TimeShow.h"
#include <QMessageBox>
#include <iostream>
#include "Cmd.h"
#include "ui_TimeShow.h"
using namespace std;

TimeShow::TimeShow(QWidget *parent) :
    QWidget(parent),
    MyTcpServer(new QTcpServer),
    ui(new Ui::TimeShow)
{
    ui->setupUi(this);
    InitUI();
    InitConnect();

    InternetPort = 51230;
    ConnectIP = nullptr;

    currentClient = nullptr;
}

void TimeShow::InitUI()
{
    ui->CloseDevice->setEnabled(false);
    ui->OpenDevice->setEnabled(true);
}

void TimeShow::InitConnect()
{
    connect(ui->OpenDevice,SIGNAL(clicked(bool)),this,SLOT(ScanInternet()));
    connect(ui->CloseDevice,SIGNAL(clicked(bool)),this,SLOT(CloseInternetConnect()));
    connect(ui->WriteTestData,SIGNAL(clicked(bool)),this,SLOT(WriteTestData()));

    connect(MyTcpServer,SIGNAL(newConnection()),this,SLOT(NewConnect()));
}


void TimeShow::ScanInternet()
{
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
    if(addressList.empty())
    {
        emit ShowParameter("None","None");
        return;
    }

    QList<QHostAddress> ipv4AddressList;
    foreach (QHostAddress address, addressList)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            ipv4AddressList.append(address);
            QString temp_str = address.toString();
            cout << "find IP:" << temp_str.toStdString() << endl;

            /* find the match ip */
            int lastDot = temp_str.lastIndexOf('.') + 1;
            int IPLastNum = temp_str.mid(lastDot,temp_str.length()).toInt();
            cout << IPLastNum <<endl;

            /* the last num is 1, the ip is  gateway */
            if(1 != IPLastNum)
            {
                ConnectIP = temp_str;
                cout << "Connect IP is:"<< ConnectIP.toStdString() << endl;
            }
        }
    }

    if(ConnectIP == nullptr)
    {
        return;
    }

    emit ShowParameter(ConnectIP,QString::number(InternetPort));
    ui->OpenDevice->setEnabled(false);
    ui->CloseDevice->setEnabled(true);
    bool is_ok = MyTcpServer->listen(QHostAddress(ConnectIP),InternetPort);
    if(is_ok)
    {
        QMessageBox::information(NULL, "info", "Listening Client", QMessageBox::Yes, QMessageBox::NoButton);
    }
}

void TimeShow::CloseInternetConnect()
{
    MyTcpServer->disconnect();
    MyTcpServer->close();

    currentClient->disconnect();
    currentClient->close();

    ui->OpenDevice->setEnabled(true);
    ui->CloseDevice->setEnabled(false);
    QMessageBox::information(NULL, "info", "close listen", QMessageBox::Yes, QMessageBox::NoButton);
}

void TimeShow::NewConnect()
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

    emit ShowSystemMessage(new_connect_info, 5000);
    cout << new_connect_info.toStdString() << endl;
}

void TimeShow::ReadData()
{
    QByteArray buffer = currentClient->readAll();
    for(int i=0;i<buffer.size();i++)
        cout << buffer.at(i) <<endl;
    RspDataProcess(buffer);
}

void TimeShow::disconnectedSlot()
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
int CalCheckSum(uint8_t* Data, uint16_t len)
{
    int CheckSum = 0;
    for(int i=0;i<len;i++)
    {
        CheckSum+=Data[i];
    }
    return CheckSum;
}

void TimeShow::RspDataProcess(QByteArray buf)
{
    uint8_t *data = (uint8_t *)buf.data();
    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)data;

    if(MSG_FRAME_HEADER != msg->header)
    {
        return;
    }

    switch(msg->cmd)
    {
        case HEART_CMD:
        {
            HeartCmdRsp();
        }
            break;
        case JUMP_CMD:
        {

        }
        break;
        default:
        {

        }
    }
}
void TimeShow::HeartCmdRsp()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = HEART_CMD;
    msg->seq = 0x00;
    msg->data_len = 1;

    server_heart_rsp_t *rsp = (server_heart_rsp_t *)(msg + 1);
    rsp->rsp_res = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t));
    uint32_t *check_sum = (uint32_t *)(rsp+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    qint64 res = currentClient->write(Sendata);
    cout << res << endl;
    if( 0 != res)
    {
        QMessageBox::warning(NULL, "warning", "send failed", QMessageBox::Yes, QMessageBox::NoButton);
    }
}

void TimeShow::WriteTestData()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = HEART_CMD;
    msg->seq = 0x00;
    msg->data_len = 1;

    server_heart_rsp_t *rsp = (server_heart_rsp_t *)(msg + 1);
    rsp->rsp_res = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(server_heart_rsp_t));
    uint32_t *check_sum = (uint32_t *)(rsp+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    qint64 res = currentClient->write(Sendata);
    if( -1 == res)
    {
        QMessageBox::warning(NULL, "warning", "send failed", QMessageBox::Yes, QMessageBox::NoButton);
    }
}
TimeShow::~TimeShow()
{
    delete ui;
}
