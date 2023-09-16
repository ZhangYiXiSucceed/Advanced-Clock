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
    MyTimeShowTimer->start(1000);
    MyNiceWordsShowTimer = new QTimer;
    MySetTimeDateTimer = new QTimer;

    InternetPort = 51230;
    ConnectIP = nullptr;
    currentClient = nullptr;
    MyNiceWordsShowTimer->start(5000);
    CurrentDateTime = QDateTime::currentDateTime();
    MySetTimeDateTimer->setSingleShot(true);

    ui->setupUi(this);
    InitUI();
    InitConnect();
    ReadNiceWordsTxt(NiceWords);
}

void TimeShow::InitUI()
{
    QFont Ft("Microsoft YaHei");
    Ft.setPointSize(15);
    ui->DateShow->setFont(Ft);
    ui->DateShow->setStyleSheet("QLineEdit{background-color:transparent}"
                                 "QLineEdit{border-width:0;border-style:outset}"
                                 "QLineEdit{color:blue}");
    Ft.setPointSize(45);
    ui->NowShow->setFont(Ft);
    ui->NowShow->setStyleSheet("QLineEdit{background-color:transparent}"
                                 "QLineEdit{border-width:0;border-style:outset}"
                                 "QLineEdit{color:red}");
    Ft.setPointSize(15);
    ui->Tempture_Humidty->setFont(Ft);
    ui->Tempture_Humidty->setStyleSheet("QLineEdit{background-color:transparent}"
                                 "QLineEdit{border-width:0;border-style:outset}"
                                 "QLineEdit{color:blue}");

    Ft.setPointSize(35);
    ui->NiceWords->setFont(Ft);
    ui->NiceWords->setStyleSheet("QTextEdit{background-color:transparent}"
                                 "QTextEdit{border-width:0;border-style:outset}"
                                 "QTextEdit{color:orange}");
    ui->NiceWords->setAlignment(Qt::AlignCenter);

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

    connect(MyTcpServer,SIGNAL(newConnection()),this,SLOT(NewConnect()));
    connect(this,SIGNAL(SetTimeReq(int,int,int)),this,SLOT(SetTime(int,int,int)));

    connect(MyTimeShowTimer,SIGNAL(timeout()),this,SLOT(TimerUpdate()));
    connect(MyNiceWordsShowTimer,SIGNAL(timeout()),this,SLOT(NiceWordsShowUpdate()));

    connect(this,SIGNAL(SetDeviceTimeDateReq(heart_data_t)),this,SLOT(SetDeviceTimeDate(heart_data_t)));
    connect(MySetTimeDateTimer,SIGNAL(timeout()),this,SLOT(UpdateSetDeviceTime()));
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
    bool is_ok = MyTcpServer->listen(QHostAddress(ConnectIP),InternetPort);
    if(is_ok)
    {
        emit ShowSystemMessage("start Listening Client",2000);
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

    emit ShowSystemMessage(new_connect_info, 10000);
    cout << new_connect_info.toStdString() << endl;

    MySetTimeDateTimer->start(5000);
    cout <<"NewConnect" <<endl;
}

void TimeShow::UpdateSetDeviceTime()
{
    emit SetDeviceTimeDateReq(weather_and_time_data_g);
    cout << "UpdateSetDeviceTime"<<endl;
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

static void MessageBoxShow(QString str)
{
    QMessageBox::warning(NULL, "warning", str, QMessageBox::Yes, QMessageBox::NoButton);
}

void TimeShow::RspDataProcess(QByteArray buf)
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
        case HEART_CMD:
        {
            uint32_t cal_sum = CalCheckSum(data,sizeof(cmd_msg_frame_t) + sizeof(heart_data_t));
            uint32_t read_sum = *((uint32_t*)(data + sizeof(cmd_msg_frame_t) + sizeof(heart_data_t)));
            if(cal_sum != read_sum)
            {
                sprintf(format_data,"frame check err,cal=%d read=%d",cal_sum,read_sum);
                QString date_str(format_data);
                MessageBoxShow(date_str);
                return;
            }
            heart_data_t* heart_data = (heart_data_t*)((cmd_msg_frame_t*)data + 1);
            weather_and_time_data_g = *heart_data;
            HeartCmdRsp();
            SetTemptureHumidty(weather_and_time_data_g.tempture,weather_and_time_data_g.humidty);
            SetWeather(weather_and_time_data_g.weather_id);
            if(MyTimeShowTimer->isActive() == false)
            {
                MyTimeShowTimer->start(1000);
            }
        }
            break;
        case SET_TIME_DATE:
        {
            cout <<"SET_TIME_DATE" <<endl;;
        }break;
        default:
        {
             emit SendData2OTA(buf);
        }
    }
}

void TimeShow::SetDeviceTimeDate(heart_data_t data)
{
    uint8_t buf[sizeof(cmd_msg_frame_t) + sizeof(time_and_date_set_t) + sizeof(uint32_t)];
    uint16_t len = sizeof(cmd_msg_frame_t) + sizeof(time_and_date_set_t) + sizeof(uint32_t);
    QByteArray Sendata;
    Sendata.resize(len);

    cmd_msg_frame_t *msg = (cmd_msg_frame_t *)buf;
    msg->header = MSG_FRAME_HEADER;
    msg->device_addr = 0x00;
    msg->cmd = SET_TIME_DATE;
    msg->seq = 0x00;
    msg->data_len = sizeof(time_and_date_set_t);

    time_and_date_set_t *set_data = (time_and_date_set_t *)(msg + 1);
    set_data->year = data.year;
    set_data->month = data.month;
    set_data->day = data.day;
    set_data->week = data.week;

    set_data->hour = data.hour;
    set_data->minute = data.minute;
    set_data->second = data.second;

    int CheckSum = CalCheckSum(buf, sizeof(cmd_msg_frame_t) + sizeof(time_and_date_set_t));
    uint32_t *check_sum = (uint32_t *)(set_data+1);
    *check_sum = CheckSum;

    memcpy((void*)Sendata.data(),buf,len);
    SendData2Device(Sendata);

    cout <<"SetDeviceTimeDate" <<endl;;
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
void TimeShow::SetTemptureHumidty(int tempture,int humidty)
{
    char buf[32];
    sprintf(buf,"T:%02d℃ -H:%02d %",tempture,humidty);
    QString date_str(buf);
    ui->Tempture_Humidty->setText(date_str);
}
void TimeShow::SetDate(int year,int month,int day,int week)
{
    char buf[32];
    sprintf(buf,"%04d-%02d-%02d %02d",year,month,day,week);
    QString date_str(buf);
    ui->DateShow->setText(date_str);
}

void TimeShow::SetTime(int hour,int minute,int second)
{
    char buf[32];
    sprintf(buf,"%02d : %02d : %02d",hour,minute,second);
    QString time_str(buf);
    ui->NowShow->setText(time_str);
}
void TimeShow::SetWeather(int weather_id)
{
    switch(weather_id)
    {
        case WEATHER_SUNNY:
        {
            ui->WeatherIcon1->setIcon(WeatherSunny);
            ui->WeatherIcon1->setIconSize(WeatherSunny.size());
        }break;
        case WEATHER_CLOUDY:
        {
            ui->WeatherIcon1->setIcon(WeatherCloudy);
            ui->WeatherIcon1->setIconSize(WeatherCloudy.size());
        }break;
        case WEATHER_WINDY:
        {
            ui->WeatherIcon1->setIcon(WeatherWindy);
            ui->WeatherIcon1->setIconSize(WeatherWindy.size());
        }break;
        case WEATHER_RAINY_AND_SNOWY:
        {
            ui->WeatherIcon1->setIcon(WeatherSnowy);
            ui->WeatherIcon1->setIconSize(WeatherSnowy.size());
        }break;
        case WEATHER_SUNNY_TO_RAINY: //go through
        case WEATHER_SMALL_THUNDER_RAINY:
        case WEATHER_LARGE_THUNDER_RAINY:
        case WEATHER_SMALL_RAINY:
        case WEATHER_MIDDLE_RAINY:
        case WEATHER_LARGE_RAINY:
        case WEATHER_MORE_LARGE_RAINY:
        {
            ui->WeatherIcon1->setIcon(WeatherRainy);
            ui->WeatherIcon1->setIconSize(WeatherRainy.size());
        }break;
        default:
            emit ShowSystemMessage(tr("weather id err!"),5000);
        break;
    }
}

void TimeShow::TimerUpdate()
{
    CurrentDateTime = QDateTime::currentDateTime();
    // 获取时分秒
    QTime time = CurrentDateTime.time();
    weather_and_time_data_g.hour = time.hour();
    weather_and_time_data_g.minute = time.minute();
    weather_and_time_data_g.second = time.second();

    // 获取年月日
    QDate date = CurrentDateTime.date();
    weather_and_time_data_g.year = date.year();
    weather_and_time_data_g.month = date.month();
    weather_and_time_data_g.day = date.day();
    weather_and_time_data_g.week = date.dayOfWeek();

    emit SetTimeReq(weather_and_time_data_g.hour,weather_and_time_data_g.minute,\
                    weather_and_time_data_g.second);
    SetDate(weather_and_time_data_g.year,weather_and_time_data_g.month,\
            weather_and_time_data_g.day,weather_and_time_data_g.week);
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
    WordsInfo.remove('\r');
    ui->NiceWords->setText(WordsInfo);
    ui->NiceWords->setAlignment(Qt::AlignCenter);
    InFile.close();
}

void TimeShow::NiceWordsShowUpdate()
{
    int WrodsSize = NiceWords.size();
    static int Index = 0;
    QString OneWords = NiceWords.at(Index);
    OneWords.remove('\r');
    ui->NiceWords->setText(OneWords);
    ui->NiceWords->setAlignment(Qt::AlignCenter);
    Index++;
    if(Index >= WrodsSize)
        Index = 0;
}
TimeShow::~TimeShow()
{
    delete ui;
}
