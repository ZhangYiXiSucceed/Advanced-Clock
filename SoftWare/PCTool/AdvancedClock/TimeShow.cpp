#include "TimeShow.h"
#include <QMessageBox>
#include <iostream>
#include <iomanip>
#include <string>
#include <sys/types.h>
#include <io.h>
#include <fstream>
#include <qtextcodec.h>
#include <QFile>
#include "Cmd.h"
#include "ui_TimeShow.h"
using namespace std;


TimeShow::TimeShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TimeShow)
{
    MyTcpServer = new QTcpServer;
    MyTimeShowTimer = new QTimer;
    MyNiceWordsShowTimer = new QTimer;

    InternetPort = 51230;
    ConnectIP = nullptr;
    currentClient = nullptr;

    ui->setupUi(this);
    InitUI();
    InitConnect();
    ReadNiceWordsTxt(NiceWords);

    MyNiceWordsShowTimer->start(5000);
}

void TimeShow::InitUI()
{
    ui->CloseDevice->setEnabled(false);
    ui->OpenDevice->setEnabled(true);

    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(25);
    ui->DateShow->setFont(Ft);
    ui->DateShow->setStyleSheet("QLineEdit{background-color:transparent}"
                                 "QLineEdit{border-width:0;border-style:outset}"
                                 "QLineEdit{color:blue}");

    ui->NowShow->setStyleSheet("QLineEdit{background-color:transparent}"
                                 "QLineEdit{border-width:0;border-style:outset}"
                                 "QLineEdit{color:red}");
    Ft.setPointSize(20);
    ui->NiceWords->setFont(Ft);
    ui->NiceWords->setStyleSheet("QLineEdit{background-color:transparent}"
                                 "QLineEdit{border-width:0;border-style:outset}"
                                 "QLineEdit{color:orange}");

    ui->WeatherIcon1->setIcon(WeatherRainy);
    ui->WeatherIcon1->setIconSize(WeatherRainy.size());
    ui->WeatherIcon1->setStyleSheet("QPushButton{background-color:transparent}"
                                 "QPushButton{border-width:0;border-style:outset}"
                                 "QPushButton{color:orange}");

    ui->WeatherIcon2->setIcon(WeatherSunny);
    ui->WeatherIcon2->setIconSize(WeatherSunny.size());
    ui->WeatherIcon2->setStyleSheet("QPushButton{background-color:transparent}"
                                 "QPushButton{border-width:0;border-style:outset}"
                                 "QPushButton{color:orange}");

    ui->WeatherIcon3->setIcon(WeatherCloudy);
    ui->WeatherIcon3->setIconSize(WeatherCloudy.size());
    ui->WeatherIcon3->setStyleSheet("QPushButton{background-color:transparent}"
                                 "QPushButton{border-width:0;border-style:outset}"
                                 "QPushButton{color:orange}");

    ui->WeatherIcon4->setIcon(WeatherSnowy);
    ui->WeatherIcon4->setIconSize(WeatherSnowy.size());
    ui->WeatherIcon4->setStyleSheet("QPushButton{background-color:transparent}"
                                 "QPushButton{border-width:0;border-style:outset}"
                                 "QPushButton{color:orange}");
    Ft.setPointSize(12);
    Ft.setBold(true);
    ui->LabelDay1->setFont(Ft);
    ui->LabelDay1->setStyleSheet("QLabel{color:green}");
    ui->LabelDay2->setFont(Ft);
    ui->LabelDay2->setStyleSheet("QLabel{color:green}");
    ui->LabelDay3->setFont(Ft);
    ui->LabelDay3->setStyleSheet("QLabel{color:green}");
    ui->LabelDay4->setFont(Ft);
    ui->LabelDay4->setStyleSheet("QLabel{color:green}");

    ui->weather_group->setFont(Ft);
    ui->TimeAndDate->setFont(Ft);
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
    connect(MyNiceWordsShowTimer,SIGNAL(timeout()),this,SLOT(NiceWordsShowUpdate()));
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

    if(currentClient != NULL)
    {
        currentClient->disconnect();
        currentClient->close();
    }

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

void TimeShow::ReadNiceWordsTxt(QList<QString> &NiceWordsList)
{
    char Buf[READMAXLENGTH];
    ifstream InFile(NICEWORDSPATH, ios::in|ios::binary);
    QString WordsInfo;
    if (!InFile)
    {
        cout <<"read NiceWordsTxT err!\n"<<endl;
        QMessageBox::warning(NULL, "warning", "open failed", QMessageBox::Yes, QMessageBox::NoButton);
        return;
    }
    while(InFile.getline(Buf, READMAXLENGTH))
    {
        WordsInfo = QString::fromLocal8Bit(Buf);
        cout << WordsInfo.toLocal8Bit().data() << endl;
        NiceWordsList.push_back(WordsInfo);
    }
    ui->NiceWords->setText(WordsInfo);
    InFile.close();
}

void TimeShow::NiceWordsShowUpdate()
{
    int WrodsSize = NiceWords.size();
    static int Index = 0;
    QString OneWords = NiceWords.at(Index);
    ui->NiceWords->setText(OneWords);
    Index++;
    if(Index >= WrodsSize)
        Index = 0;
}
TimeShow::~TimeShow()
{
    delete ui;
}
