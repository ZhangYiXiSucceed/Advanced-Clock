#include "TimeShow.h"
#include <QMessageBox>
#include <iostream>
#include <iomanip>
#include "Cmd.h"
#include "ui_TimeShow.h"
using namespace std;


TimeShow::TimeShow(QWidget *parent) :
    QWidget(parent),
    MyTcpServer(new QTcpServer),
    MyTimeShowTimer(new QTimer),
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

    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(25);
    ui->DateShow->setStyleSheet("color:blue");
    ui->DateShow->setFont(Ft);

    ui->NowShow->setStyleSheet("color:red");
}

void TimeShow::InitConnect()
{
    connect(ui->OpenDevice,SIGNAL(clicked(bool)),this,SLOT(ScanInternet()));
    connect(ui->CloseDevice,SIGNAL(clicked(bool)),this,SLOT(CloseInternetConnect()));
    connect(ui->WriteTestData,SIGNAL(clicked(bool)),this,SLOT(WriteTestData()));
    connect(ui->ConnectDevice,SIGNAL(clicked(bool)),this,SLOT(ConnectCmd()));

    connect(MyTcpServer,SIGNAL(newConnection()),this,SLOT(NewConnect()));
    connect(this,SIGNAL(SetTimeReq(int,int,int)),this,SLOT(SetTime(int,int,int)));

    connect(MyTimeShowTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
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
            /*cout << IPLastNum <<endl;*/

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
    QString disp_string,S;
    QByteArray buffer = currentClient->readAll();
    cout <<"len=" <<buffer.size()<<endl;;

    for(int i=0;i<buffer.size();i++)
    {
        S.sprintf("0x%02x, ", (unsigned char)buffer.at(i));
        disp_string += S;
    }

    cout << disp_string.toStdString()<<endl;
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
        cout <<"header err" << msg->header << endl;
        return;
    }

    switch(msg->cmd)
    {
        case HEART_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + sizeof(heart_data_t));
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + sizeof(heart_data_t)));
            if(cal_sum != read_sum)
            {
                cout <<"frame check err,cal=" << cal_sum <<"read= "<< cal_sum << endl;
                return;
            }
            heart_data_t* heart_data = (heart_data_t*)((cmd_msg_frame_t*)data + 1);
            weather_and_time_data_g = *heart_data;
            HeartCmdRsp();
            SetDate(weather_and_time_data_g.year,weather_and_time_data_g.month,\
                    weather_and_time_data_g.day,weather_and_time_data_g.week);
            if(MyTimeShowTimer->isActive() == false)
            {
                MyTimeShowTimer->start(1000);
            }
        }
            break;
        case JUMP_CMD:
        {
            QMessageBox::information(NULL, "info", "jump ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case CONNECT_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + 1);
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + 1));
            if(cal_sum != read_sum)
            {
                cout <<"frame check err,cal=" << cal_sum <<"read= "<< cal_sum << endl;
                return;
            }
            QMessageBox::information(NULL, "info", "connect ok", QMessageBox::Yes, QMessageBox::NoButton);
        }
        break;
        case Reset_Cmd:
        {
            emit SendData2OTA(buf);
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
    SendData2Device(Sendata);
}

void TimeShow::WriteTestData()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = JUMP_CMD;
    msg->seq = 0x00;
    msg->data_len = 0x00;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    SendData2Device(Sendata);
}

void TimeShow::ConnectCmd()
{
    uint8_t buf[sizeof(cmd_msg_frame_t) +  sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = CONNECT_CMD;
    msg->seq = 0x00;
    msg->data_len = 0;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t));
    uint32_t *check_sum = (uint32_t *)(msg+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    SendData2Device(Sendata);
}

void TimeShow::SendData2Device(QByteArray Data)
{
    qint64 res = currentClient->write(Data);
    if( -1 == res)
    {
        QMessageBox::warning(NULL, "warning", "send failed", QMessageBox::Yes, QMessageBox::NoButton);
    }
}

void TimeShow::SetDate(int year,int month,int day,int week)
{
    QString date_str = tr("20");
    date_str += QString::number(year);
    date_str += tr("--");

    date_str += QString::number(month);
    date_str += tr("--");

    date_str += QString::number(day);

    ui->DateShow->setText(date_str);
}

void TimeShow::SetTime(int hour,int minute,int second)
{
    QString time_str ;
    time_str += QString::number(hour);
    time_str += tr(":");

    time_str += QString::number(minute);
    time_str += tr(":");

    time_str += QString::number(second);
    ui->NowShow->setText(time_str);
}

void TimeShow::TimerUpdate()
{
    weather_and_time_data_g.second++;
    if(weather_and_time_data_g.second>=60)
    {
        weather_and_time_data_g.second = 0;
        weather_and_time_data_g.minute++;
        if(weather_and_time_data_g.minute>=60)
        {
            weather_and_time_data_g.minute = 0;
            weather_and_time_data_g.hour++;
        }
    }
    emit SetTimeReq(weather_and_time_data_g.hour,weather_and_time_data_g.minute,\
                    weather_and_time_data_g.second);

}
TimeShow::~TimeShow()
{
    delete ui;
}
