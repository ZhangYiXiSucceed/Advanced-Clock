#ifndef TIMESHOW_H
#define TIMESHOW_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QTimer>
#include <string>
#include <QDateTime>
#include <sys/types.h>
#include <io.h>
#include <fstream>
#include "Cmd.h"

#define NICEWORDSPATH "D:\\Workspace\\DesignProject\\AdvancedClock\\Advanced-Clock\\SoftWare\\PCTool\\AdvancedClock\\NiceWords.txt"
#define READMAXLENGTH  512
namespace Ui {
class TimeShow;
}

class TimeShow : public QWidget
{
    Q_OBJECT

public:
    explicit TimeShow(QWidget *parent = nullptr);
    ~TimeShow();
    void InitUI();
    void InitConnect();
    void RspDataProcess(QByteArray buf);
    void HeartCmdRsp();
    void ReadNiceWordsTxt(QList<QString> &NiceWordsList);
signals:
    void ShowParameter(QString, QString);
    void ShowSystemMessage(QString,uint16_t);
    void SendData2OTA(QByteArray Data);
    void SetTimeReq(int hour,int minute,int second);
    void SetDeviceTimeDateReq(heart_data_t data);
private slots:
    void ScanInternet();
    void CloseInternetConnect();
    void NewConnect();
    void ReadData();
    void disconnectedSlot();
    void WriteTestData();
    void ConnectCmd();
    void SendData2Device(QByteArray Data);
    void SetDate(int year,int month,int day,int week);
    void SetTime(int hour,int minute,int second);
    void SetTemptureHumidty(int tempture,int humidty);
    void TimerUpdate();
    void NiceWordsShowUpdate();
    void SetWeather(int Weather_id);
    void SetDeviceTimeDate(heart_data_t data);
    void UpdateSetDeviceTime();
private:
    Ui::TimeShow *ui;
    QTcpServer *MyTcpServer;
    QList<QTcpSocket*> MyTcpClient;
    QTcpSocket *currentClient;

    QTimer  *MyTimeShowTimer;
    QTimer  *MyNiceWordsShowTimer;
    QTimer  *MySetTimeDateTimer;

    int InternetPort;
    QString ConnectIP;
    heart_data_t weather_and_time_data_g;

    QList<QString> NiceWords;

    QDateTime CurrentDateTime = QDateTime::currentDateTime();

    QPixmap WeatherCloudy=tr(":/Image/cloudy.png");
    QPixmap WeatherRainy=tr(":/Image/rainy.png");
    QPixmap WeatherSnowy=tr(":/Image/snowy.png");
    QPixmap WeatherSunny=tr(":/Image/sunny.png");
    QPixmap WeatherWindy=tr(":/Image/windy.png");
};

#endif // TIMESHOW_H
